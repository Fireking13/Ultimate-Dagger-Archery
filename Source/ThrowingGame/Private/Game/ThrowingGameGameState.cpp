// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/ThrowingGameGameState.h"
#include "Game/MissionHub.h"
#include "Kismet/GameplayStatics.h"
#include "TargetObjectPoolComponent.h"

AThrowingGameGameState::AThrowingGameGameState()
{
	PrimaryActorTick.bCanEverTick = true;

	m_TargetPool = CreateDefaultSubobject<UTargetObjectPoolComponent>(TEXT("TargetObjectPoolComponent"));
}

void AThrowingGameGameState::BeginPlay()
{
	Super::BeginPlay();

	if (m_MissionHub == nullptr)
	{
		TArray<AActor*> FoundMissionHubs;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMissionHub::StaticClass(), FoundMissionHubs);

		if (FoundMissionHubs.Num() > 0)
		{
			m_MissionHub = Cast<AMissionHub>(FoundMissionHubs[0]);
		}
		else
		{
			//error
		}
	}

	StartLevel(); //debug TODO: remove
}

void AThrowingGameGameState::FReceivePointsHandler(int32 points, int32 pointsOG, float distanceVal, float airTimeStyle, float speedVal)
{
	m_Score += points;
}

void AThrowingGameGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AThrowingGameGameState::StartLevel()
{
	if (m_MissionHub)
	{
		m_MissionHub->StartUp();
	}
}

void AThrowingGameGameState::EndLevel()
{
	if (m_MissionHub)
	{
		m_MissionHub->SetIsActive(false);
	}
}

AMissionHub* AThrowingGameGameState::GetMissionHub()
{
	return m_MissionHub;
}

UTargetObjectPoolComponent* AThrowingGameGameState::GetTargetPool()
{
	return m_TargetPool;
}
