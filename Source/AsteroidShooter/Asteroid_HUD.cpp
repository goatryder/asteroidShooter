// Fill out your copyright notice in the Description page of Project Settings.


#include "Asteroid_HUD.h"

#include "Asteroid.h"
#include "SpaceShipPawn.h"

void AAsteroid_HUD::DrawHUD() {
	FString ScoreString = FString("Score: " + FString::FromInt(AAsteroid::AsteroidScore));
	DrawText(ScoreString, FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 0.0f, nullptr, 3);

	FString ShotsFired = FString("Shots Fired: " + FString::FromInt(ASpaceShipPawn::ShotsFired));
	DrawText(ShotsFired, FLinearColor(1.0f, 1.0f, 1.0f, 1.0f), 0.0f, 30.0f, nullptr, 3);



}