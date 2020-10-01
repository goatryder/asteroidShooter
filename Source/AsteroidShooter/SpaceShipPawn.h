// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "Laser.h"

#include "SpaceShipPawn.generated.h"

UCLASS()
class ASTEROIDSHOOTER_API ASpaceShipPawn : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* PlaneMesh;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Cannon;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ALaser> Laser;


public:
	// Sets default values for this pawn's properties
	ASpaceShipPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, 
		bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

	static int ShotsFired;

private:
	
	FVector LocalVelocity;

	float CurrentYawSpeed;
	float CurrentPitchSpeed;
	float CurrentRollSpeed;

protected:

	UPROPERTY(EditAnywhere)
	int BoundaryRadius;

	UPROPERTY(EditAnywhere)
	float Acceleration;

	UPROPERTY(EditAnywhere)
	float TurnSpeed;

	UPROPERTY(EditAnywhere)
	float MaxSpeed;

	UPROPERTY(EditAnywhere)
	float MinSpeed;

	UPROPERTY(EditAnywhere)
	float SlowDown;

	// check if collision occurs with world boundary or obstacles
	FVector CheckAndDoBoundaryHit(FVector WorldLocation, FVector Velocity) {

		if (WorldLocation.Size() < BoundaryRadius)
			return Velocity;

		Velocity = Velocity - 2 * FVector::DotProduct(WorldLocation, Velocity) * WorldLocation / WorldLocation.Size();

		return Velocity;

	}

	void ThrustInput(float Value);
	void RotateCannonUp(float Value);
	void RotateCannonRight(float Value);
	void FireCannon(float Value);

};
