// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/ThrowingGameGameState.h"
#include "Game/MissionHub.h"
#include "Kismet/GameplayStatics.h"
#include "TargetObjectPoolComponent.h"
#include "Game/PointDisplayComponent.h"

AThrowingGameGameState::AThrowingGameGameState()
{
	PrimaryActorTick.bCanEverTick = true;

	m_TargetPool = CreateDefaultSubobject<UTargetObjectPoolComponent>(TEXT("TargetObjectPoolComponent"));

	m_PointDisplay = CreateDefaultSubobject<UPointDisplayComponent>(TEXT("PointDisplayComponent"));

	DoScoreEffect = false; 
	ScoreSizeMult = 1.f;
	ScoreSizeMax = 2.f;
	ScoreEffectTime = 0.f;
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
	m_PointDisplay->AddToList(points, pointsOG, distanceVal, airTimeStyle, speedVal);

	m_Score += points; //TODO delay
}

void AThrowingGameGameState::FPointsEffectHandler(float effectTime) //move this after is works TODO:
{
	ScoreEffectTime = effectTime;
	ScoreEffectTimer = ScoreEffectTime;
	DoScoreEffect = true;
}

void AThrowingGameGameState::CalScoreSize(float DeltaTime)
{
	float timeNormalized = 1.f - (ScoreEffectTimer / ScoreEffectTime);
	float height = ScoreSizeMax - 1.f;

	ScoreSizeMult = 1.f + 4.f * height * timeNormalized * (1.f - timeNormalized);

	ScoreEffectTimer -= DeltaTime;

	if (ScoreEffectTimer <= 0.f)
	{
		DoScoreEffect = false; 
		ScoreEffectTime = 0.f;
		ScoreEffectTimer = 0.f;
	}
}

void AThrowingGameGameState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DoScoreEffect)
	{
		CalScoreSize(DeltaTime);
	}
}

void AThrowingGameGameState::StartLevel()
{
	if (m_MissionHub)
	{
		m_MissionHub->StartUp();
	}

	DoScoreEffect = false;

	m_PointDisplay->GetPointsEffectHandler().AddDynamic(this, &AThrowingGameGameState::FPointsEffectHandler);
	
}

void AThrowingGameGameState::EndLevel()
{
	if (m_MissionHub)
	{
		m_MissionHub->SetIsActive(false);
	}
}

