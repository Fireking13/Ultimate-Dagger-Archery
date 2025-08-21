// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MissionBase.h"

// Sets default values
AMissionBase::AMissionBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsActive = false;
}

// Called when the game starts or when spawned
void AMissionBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMissionBase::FOnTargetDeactivationHandler()
{
	IsActive = false;

	m_Target = nullptr;

	OnMissionComplete.Broadcast();
	//OnMissionComplete.Clear();
}

// Called every frame
void AMissionBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMissionBase::StartUp(TArray<FVector> points, AActor* actorWithSpline, float speed, bool faceOutwards)
{
}

