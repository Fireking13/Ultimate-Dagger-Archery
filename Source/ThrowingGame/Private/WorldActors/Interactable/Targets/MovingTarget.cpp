// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldActors/Interactable/Targets/MovingTarget.h"

AMovingTarget::AMovingTarget()
{
	PrimaryActorTick.bCanEverTick = true;

	InbetweenPointsNum = 50;
}

void AMovingTarget::BeginPlay()
{
	Super::BeginPlay();
}

void AMovingTarget::MakeFullPath()
{
}

void AMovingTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMovingTarget::SetMainTrailPoints(TArray<FVector> mainTrailPoints)
{
	MainTrailPoints = mainTrailPoints;
}
