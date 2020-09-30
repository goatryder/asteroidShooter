// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Engine/Engine.h"

#include "Laser.generated.h"

UCLASS()
class ASTEROIDSHOOTER_API ALaser : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALaser();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp,
		bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

private:

	UPROPERTY(EditAnywhere)
	float LaunchSpeed;

	UPROPERTY(EditAnywhere)
	float SurvivalTime;

	FVector Direction;
	float TimeElapsed;

public:

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* LaserMesh;

	UPROPERTY(EditAnywhere)
	class UAudioComponent* Sound;

	inline void SetLaunchSpeed(float Input) { LaunchSpeed = Input; }

};
