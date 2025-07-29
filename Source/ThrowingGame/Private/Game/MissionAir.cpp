// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MissionAir.h"
#include "WorldActors/Interactable/Targets/BaseTarget.h"
#include "Game/ThrowingGameGameState.h"
#include "TargetObjectPoolComponent.h"

AMissionAir::AMissionAir()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMissionAir::BeginPlay()
{
	Super::BeginPlay();
}

void AMissionAir::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMissionAir::StartUp(TArray<FVector> points)
{
	AGameStateBase* baseGameState = GetWorld()->GetGameState();

	AThrowingGameGameState* gameState = Cast<AThrowingGameGameState>(baseGameState);

	if (gameState)
	{
		m_Target = gameState->GetTargetPool()->GetABaseTarget();

		m_Target->GetTargetHandler().AddDynamic(this, &AMissionAir::FOnTargetDeactivationHandler);
		m_Target->Spawn(GetActorLocation(), GetActorRotation());
	}
}
