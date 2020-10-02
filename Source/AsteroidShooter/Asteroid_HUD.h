// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Asteroid_HUD.generated.h"

/**
 * 
 */
UCLASS()
class ASTEROIDSHOOTER_API AAsteroid_HUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;
	
};
