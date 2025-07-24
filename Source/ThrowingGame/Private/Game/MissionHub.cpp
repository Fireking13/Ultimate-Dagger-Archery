// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MissionHub.h"
#include "Game/MissionPoint.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AMissionHub::AMissionHub()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMissionHub::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMissionHub::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMissionHub::Start()
{
	for (int32 i = 0; i < NumActiveMissions; i++)
	{

	}
}

void AMissionHub::Start()
{

}
