// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MissionSpline.h"

AMissionSpline::AMissionSpline()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMissionSpline::BeginPlay()
{
	Super::BeginPlay();
}

void AMissionSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMissionSpline::StartUp(FVector pos)
{
}
