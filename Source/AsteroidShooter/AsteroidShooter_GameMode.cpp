// Fill out your copyright notice in the Description page of Project Settings.


#include "AsteroidShooter_GameMode.h"

#include "SpaceShipPawn.h"

// Sets default values
AAsteroidShooter_GameMode::AAsteroidShooter_GameMode() {

	DefaultPawnClass = ASpaceShipPawn::StaticClass();

} // constructor