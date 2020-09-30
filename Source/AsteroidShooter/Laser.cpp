// Fill out your copyright notice in the Description page of Project Settings.


#include "Laser.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
// #include "Engine/World.h"
//#include "Engine/StaticMesh.h"

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
		TEXT("/Game/Geometry/Material/LaserMaterial.LaserMaterial")).Get();
	
	LaserMesh->SetMaterial(int32(), TempMaterial);

	Sound = CreateDefaultSubobject<UAudioComponent>(TEXT("Laser Sound"));
	// Sound->SetSound(ConstructorStatics.LaserSound.Get());
	// Sound->Play();


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

}

void ALaser::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, 
	bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
}

