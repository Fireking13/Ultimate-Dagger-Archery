// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MissionSideToSide.h"

AMissionSideToSide::AMissionSideToSide()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMissionSideToSide::BeginPlay()
{
	Super::BeginPlay();
}

void AMissionSideToSide::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMissionSideToSide::StartUp(FVector pos)
{
}
