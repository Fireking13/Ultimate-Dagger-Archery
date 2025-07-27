// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MissionGround.h"

AMissionGround::AMissionGround()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMissionGround::BeginPlay()
{
	Super::BeginPlay();
}

void AMissionGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMissionGround::StartUp(FVector pos)
{
}
