// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/ThrowingGameGameState.h"
#include "Game/MissionHub.h"

AThrowingGameGameState::AThrowingGameGameState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AThrowingGameGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel(); //debug TODO: remove
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
