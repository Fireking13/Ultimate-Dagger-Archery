// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MissionSpline.h"
#include "WorldActors/Interactable/Targets/SplineTarget.h"
#include "Game/ThrowingGameGameState.h"
#include "TargetObjectPoolComponent.h"

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

void AMissionSpline::StartUp(TArray<FVector> points)
{
	Points = points;

	AGameStateBase* baseGameState = GetWorld()->GetGameState();

	AThrowingGameGameState* gameState = Cast<AThrowingGameGameState>(baseGameState);

	if (gameState)
	{
		ASplineTarget* target = gameState->GetTargetPool()->GetASplineTarget();
		
		target->SetUpSpline(Points);
		target->GetTargetHandler().AddDynamic(this, &AMissionSpline::FOnTargetDeactivationHandler);
		target->Spawn(GetActorLocation(), GetActorRotation());

		m_Target = target;
	}
}
