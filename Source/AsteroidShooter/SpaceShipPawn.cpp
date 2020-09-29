// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShipPawn.h"

#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"

//#include "Laser.h"
//#include "Asteroid.h"

int ASpaceShipPawn::ShotsFired = 0;

// Sets default values
ASpaceShipPawn::ASpaceShipPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// structure to hold one time initialization
	struct FConstructorStatics {

		// declare which object  you want to find
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh, Cannon;

		FConstructorStatics() :
			PlaneMesh(TEXT("/Game/Geometry/Meshes/SpaceShip2.Spaceship2")),
			Cannon(TEXT("/Game/Geometry/Meshes/Turret1.Turret1"))
		{}

	};

	Tags.Add("Player");

	static FConstructorStatics ConstructorStatics;

	// create static mesh component for ship
	PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane Mesh"));
	PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	PlaneMesh->SetWorldScale3D(FVector(0.25f, 0.25f, 0.25f));

	RootComponent = PlaneMesh;

	// create static mesh for cannon
	Cannon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon"));
	Cannon->SetStaticMesh(ConstructorStatics.Cannon.Get());
	Cannon->SetRelativeLocation(FVector(-200.0f, 0.0f, 85.0f));
	Cannon->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
	Cannon->SetupAttachment(PlaneMesh);

	// create spring arm component
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Cannon);

	// the camera follow at this distance behind the character
	SpringArm->TargetArmLength = 160.0f;
	SpringArm->SocketOffset = FVector(-70.0f, 0.0f, 50.0f);
	SpringArm->bEnableCameraLag = false; //don't allow camera to log
	SpringArm->CameraLagSpeed = 15.0f;

	// create camera component
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false; // don't rotate camera with controller

	// Initial values
	Acceleration = 500.0f;
	TurnSpeed = 100.0f;
	MaxSpeed = 1000.0f;
	MinSpeed = 500.0f;
	LocalVelocity = FVector(0.0, 0.0, 0.0);
	SlowDown = 100.0f;
	BoundaryRadius = 10000;


} // constructor

// Called when the game starts or when spawned
void ASpaceShipPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpaceShipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector LocalMove = LocalVelocity * DeltaTime;
	
	AddActorLocalOffset(LocalMove, true);

	FRotator DeltaRotation = Cannon->GetComponentRotation() - RootComponent->GetComponentRotation();

	DeltaRotation.Normalize();

	DeltaRotation.Yaw = abs(DeltaRotation.Yaw) <= TurnSpeed * DeltaTime ? 
		DeltaRotation.Yaw : 
		DeltaRotation.Yaw / abs(DeltaRotation.Yaw) * TurnSpeed * DeltaTime;

	DeltaRotation.Pitch = abs(DeltaRotation.Pitch) <= TurnSpeed * DeltaTime ? 
		DeltaRotation.Pitch :
		DeltaRotation.Pitch / abs(DeltaRotation.Pitch) * TurnSpeed * DeltaTime;

	DeltaRotation.Roll = abs(DeltaRotation.Roll) <= TurnSpeed * DeltaTime ?
		DeltaRotation.Roll :
		DeltaRotation.Roll / abs(DeltaRotation.Roll) * TurnSpeed * DeltaTime;

	AddActorLocalRotation(DeltaRotation);

	LocalVelocity = DeltaRotation.UnrotateVector(LocalVelocity);

	LocalVelocity = LocalVelocity.GetClampedToMaxSize(MaxSpeed);
	Cannon->AddLocalRotation(-1 * DeltaRotation);

	// SPAWN ASTEROIDS

	LocalVelocity = CheckAndDoBoundaryHit(GetActorLocation(), GetActorRotation().RotateVector(LocalVelocity));

	LocalVelocity = GetActorRotation().UnrotateVector(LocalVelocity);  // inverse rotation

} // tick

// Called to bind functionality to input
void ASpaceShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// check if player input comp is valid e.g. not equal to NULL
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Thrust", this, &ASpaceShipPawn::ThrustInput);
	PlayerInputComponent->BindAxis("CannonUp", this, &ASpaceShipPawn::RotateCannonUp);
	PlayerInputComponent->BindAxis("CannonRight", this, &ASpaceShipPawn::RotateCannonRight);
	PlayerInputComponent->BindAxis("FireCannon", this, &ASpaceShipPawn::FireCannon);

}

void ASpaceShipPawn::ThrustInput(float Value)
{

	float DeltaV = Value * Acceleration * GetWorld()->GetDeltaSeconds();

	// slow ship over time
	FVector SlowedVelocity = LocalVelocity - SlowDown * LocalVelocity / LocalVelocity.GetAbs() * GetWorld()->GetDeltaSeconds();

	if (Dot3(LocalVelocity, SlowedVelocity) > 0)
		LocalVelocity = SlowedVelocity;
	
	LocalVelocity = LocalVelocity + FVector(DeltaV, 0.0f, 0.0f);

}

void ASpaceShipPawn::RotateCannonUp(float Value)
{

	Cannon->AddLocalRotation(FRotator(Value, 0.0f, -Cannon->GetComponentRotation().Roll));

}

void ASpaceShipPawn::RotateCannonRight(float Value)
{

	Cannon->AddLocalRotation(FRotator(0.0f, Value, -Cannon->GetComponentRotation().Roll));

}

void ASpaceShipPawn::FireCannon(float Value)
{
}

void ASpaceShipPawn::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
}
