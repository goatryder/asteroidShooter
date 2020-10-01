// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Asteroid.generated.h"


UCLASS()
class ASTEROIDSHOOTER_API AAsteroid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsteroid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp,
		bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

public:

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* AsteroidMesh;

	UPROPERTY(EditAnywhere)
	class UParticleSystemComponent* ExplosionParticleComponent;

	UPROPERTY(EditAnywhere)
	class UAudioComponent* Sound;

	static int AsteroidScore;

	inline void SetSizeCategory(int NewSize) {

		SetActorScale3D(FVector(1 << NewSize));

		SizeCategory = NewSize;

	}

	inline void SetRandomVelocity() {

		AsteroidVelocity = FVector(rand() % 101 - 50, rand() % 101 - 50, rand() % 101 - 50);
		AsteroidVelocity.Normalize();
		AsteroidVelocity = (rand() % int(SpeedModifier / SizeCategory - MinVelocity) + 1 + MinVelocity) * AsteroidVelocity;

	}

protected:

	UPROPERTY(EditAnywhere)
	int SizeCategory;

	UPROPERTY(EditAnywhere)
	float SpeedModifier;

	UPROPERTY(EditAnywhere)
	float MinVelocity;

	UPROPERTY(EditAnywhere)
	float AsteroidBoundary;
	
	// check if collision occurs with world boundary or obstacles
	FVector CheckAndDoBoundaryHit(FVector WorldLocation, FVector PassedVelocity) {
		
		if (abs(WorldLocation.Size()) > AsteroidBoundary)
			return PassedVelocity;

		return PassedVelocity - 2 * (FVector::DotProduct(WorldLocation, PassedVelocity)) *
			WorldLocation / WorldLocation.Size();

	}

private:

	FVector AsteroidVelocity;

	bool isFree;

	void Explode();

};
