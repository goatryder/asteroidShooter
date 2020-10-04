// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroid.h"

// #include "Engine/World.h"
// #include "Engine/StaticMesh.h"

#include "UObject/ConstructorHelpers.h"

#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Kismet/GameplayStatics.h"

#include "AsteroidShooter_GameMode.h"

// #include "ParticleHelper.h"
// #include "Particles/ParticleSystem.h"


int AAsteroid::AsteroidScore = 0;

// Sets default values
AAsteroid::AAsteroid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// create actor components
	struct FConstructorStatics {
		
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> AsteroidMesh;
		ConstructorHelpers::FObjectFinderOptional<UParticleSystem> ExplosionFX;
		ConstructorHelpers::FObjectFinderOptional<USoundBase> ExplosionSound;

		FConstructorStatics() :
			AsteroidMesh(TEXT("/Game/Geometry/Meshes/Asteroid1.Asteroid1")),
			ExplosionFX(TEXT("/Game/FXVarietyPack/Particles/P_ky_explosion.P_ky_explosion")),
			ExplosionSound(TEXT("/Game/Sound/boom.boom"))
		
		{
		}

	};

	static FConstructorStatics ConstructorStatics;

	AsteroidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Asteroid Mesh"));
	AsteroidMesh->SetStaticMesh(ConstructorStatics.AsteroidMesh.Get());

	RootComponent = AsteroidMesh;

	Tags.Add("Asteroid");

	ExplosionParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionFX"));
	ExplosionParticleComponent->SetTemplate(ConstructorStatics.ExplosionFX.Get());
	ExplosionParticleComponent->SetWorldLocation(AsteroidMesh->GetComponentLocation());
	ExplosionParticleComponent->Deactivate();

	Sound = CreateDefaultSubobject<UAudioComponent>(TEXT("Boom Sound"));
	Sound->SetSound(ConstructorStatics.ExplosionSound.Get());
	Sound->Deactivate();

	// Initial values
	MinVelocity = 10.0f;
	SpeedModifier = 10000.0f;
	SizeCategory = 3;
	
	AsteroidBoundary = 50000.0f;

	SetSizeCategory(SizeCategory);
	SetRandomVelocity();

} // constructor

// Called when the game starts or when spawned
void AAsteroid::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAsteroid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// move asteroid
	AddActorLocalOffset(DeltaTime * AsteroidVelocity, true);
	
	
	//AsteroidVelocity = CheckAndDoBoundaryHit(
		//GetActorLocation(), GetActorRotation().RotateVector(AsteroidVelocity));

	// explode if leave boundary
	if (GetActorLocation().Size() > AsteroidBoundary)
		Explode();

}

void AAsteroid::Explode()
{
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), Sound->Sound, GetActorLocation());

	// make particles explode
	ExplosionParticleComponent->AddLocalOffset(-1 * ExplosionParticleComponent->GetRelativeLocation());
	ExplosionParticleComponent->ResetParticles(true);
	ExplosionParticleComponent->Activate(true);

	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(), ExplosionParticleComponent->Template,
		FTransform(GetActorRotation(), GetActorLocation(), GetActorScale3D())
	);

	if (SizeCategory > 0) {

		AAsteroidShooter_GameMode* GameModeREF = Cast<AAsteroidShooter_GameMode>(UGameplayStatics::GetGameMode(GetWorld()));
		TArray<TSubclassOf<AAsteroid>>Asteroids = GameModeREF->Asteroids;
		TSubclassOf<AAsteroid> AsteroidClass = Asteroids[rand() % Asteroids.Num()]; // Get random asteroid class

		for (int i = 0; i < 4; i++) {

			AAsteroid* Temp = GetWorld()->SpawnActor<AAsteroid>(AsteroidClass, GetActorLocation(), GetActorRotation() + FRotator(rand() % 360));

			Temp->SetSizeCategory(SizeCategory - 1); // spawn one power smaller asteroid

			Temp->SetRandomVelocity();  // SizeCategory changed, reset velocity

		}
	
	}

	Destroy();

	AAsteroid::AsteroidScore++;

}

void AAsteroid::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{

	if (Other->ActorHasTag("Laser"))
		Explode();

	else if (Other->ActorHasTag("Asteroid")) {

		if (!isFree)
			AddActorWorldOffset(AsteroidVelocity * 0.1f, false);
		else
			AsteroidVelocity = AsteroidVelocity - 2 * (FVector::DotProduct(HitNormal, AsteroidVelocity)) * HitNormal.Size();

	}

	else {

		AsteroidVelocity = AsteroidVelocity - 2 * (FVector::DotProduct(HitNormal, AsteroidVelocity)) * HitNormal / HitNormal.Size();

	}

}