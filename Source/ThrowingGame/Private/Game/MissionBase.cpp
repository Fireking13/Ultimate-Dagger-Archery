// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MissionBase.h"

// Sets default values
AMissionBase::AMissionBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	isNotDone = false;
}

// Called when the game starts or when spawned
void AMissionBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMissionBase::FOnTargetDeactivationHandler()
{
	isNotDone = true;

	OnMissionComplete.Broadcast();
}

// Called every frame
void AMissionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMissionBase::StartUp(TArray<FVector> points)
{
}

