// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroid.h"

// #include "Engine/World.h"
// #include "Engine/StaticMesh.h"

#include "UObject/ConstructorHelpers.h"

#include "Components/StaticMeshComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "Kismet/GameplayStatics.h"

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
	MinVelocity = 10;
	SpeedModifier = 10000;
	SizeCategory = 3;

	RandomVelocity();
	SetSizeCategory(SizeCategory);

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

}

void AAsteroid::Explode()
{
}

void AAsteroid::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
}