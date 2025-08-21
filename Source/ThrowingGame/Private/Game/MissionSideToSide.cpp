// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MissionSideToSide.h"
#include "WorldActors/Interactable/Targets/SideToSideTarget.h"
#include "Game/ThrowingGameGameState.h"
#include "TargetObjectPoolComponent.h"

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

void AMissionSideToSide::StartUp(TArray<FVector> points, AActor* actorWithSpline, float speed, bool faceOutwards)
{
	Speed = speed;
	Points = points;

	AGameStateBase* baseGameState = GetWorld()->GetGameState();

	AThrowingGameGameState* gameState = Cast<AThrowingGameGameState>(baseGameState);

	if (gameState)
	{
		ASideToSideTarget* target = gameState->GetTargetPool()->GetASideToSideTarget();

		target->SetUpPoints(Points);
		target->SetSpeed(speed); //TODO remove this move logic to mission

		target->GetTargetHandler().Clear(); //.RemoveDynamic(this, &AMissionSideToSide::FOnTargetDeactivationHandler);
		target->GetTargetHandler().AddDynamic(this, &AMissionSideToSide::FOnTargetDeactivationHandler);
		target->Spawn(GetActorLocation(), GetActorRotation());

		m_Target = target;
	}

	IsActive = true;
}
