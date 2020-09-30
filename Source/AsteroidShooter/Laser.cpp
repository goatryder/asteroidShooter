// Fill out your copyright notice in the Description page of Project Settings.


#include "Laser.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
ALaser::ALaser()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	struct FConstructorStatics {

		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> LaserMesh;
		ConstructorHelpers::FObjectFinderOptional<USoundBase> LaserSound;

		FConstructorStatics() :
			LaserMesh(TEXT("/Game/Geometry/Meshes/Laser.Laser")),
			LaserSound(TEXT("/Game/Sound/Laser.Laser"))

		{}
	
	};

	static FConstructorStatics ConstructorStatics;

	LaserMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Laser Mesh"));
	LaserMesh->SetStaticMesh(ConstructorStatics.LaserMesh.Get());
	LaserMesh->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));
	RootComponent = LaserMesh;

	// add material to mesh
	UMaterial* TempMaterial = CreateDefaultSubobject<UMaterial>(TEXT("Laser Material"));
	TempMaterial = ConstructorHelpers::FObjectFinderOptional<UMaterial>(
		TEXT("/Game/Geometry/Materials/LaserMaterial.LaserMaterial")).Get();
	
	LaserMesh->SetMaterial(int32(), TempMaterial);

	Sound = CreateDefaultSubobject<UAudioComponent>(TEXT("Laser Sound"));
	Sound->SetSound(ConstructorStatics.LaserSound.Get());
	Sound->Play();

	// Initial values
	Direction = FVector(1.0f, 0.0f, 0.0f);
	LaunchSpeed = 1000.0f;
	SurvivalTime = 5.0f;
	TimeElapsed = 0.0f;

	Tags.Add(TEXT("Laser"));


} // constructor

// Called when the game starts or when spawned
void ALaser::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALaser::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeElapsed += DeltaTime;

	if (TimeElapsed > SurvivalTime)
		Destroy();

	const FVector LocalMove = Direction * DeltaTime * LaunchSpeed;

	AddActorLocalOffset(LocalMove, true);

} // tick

void ALaser::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, 
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{

	if (!Other->ActorHasTag("Player"))
		Destroy();
	else
		AddActorLocalOffset(FVector(10.0f, 0.0f, 0.0f), false);

}

