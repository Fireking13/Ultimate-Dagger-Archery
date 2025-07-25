// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MissionPoint.h"
#include "Game/MissionBase.h"

// Sets default values
AMissionPoint::AMissionPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Mission = nullptr;

	IsActive = false;
}

// Called when the game starts or when spawned
void AMissionPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMissionPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMissionPoint::StartUp()
{

	if (m_Mission)
	{
		//m_Mission->StartUp();
	}
	else
	{

	}
}

bool AMissionPoint::GetIsActive()
{
	return IsActive;
}
