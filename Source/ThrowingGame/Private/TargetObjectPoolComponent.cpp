// Fill out your copyright notice in the Description page of Project Settings.

#include "TargetObjectPoolComponent.h"
#include "WorldActors/Interactable/Targets/BaseTarget.h"
#include "WorldActors/Interactable/Targets/SplineTarget.h"
#include "WorldActors/Interactable/Targets/SideToSideTarget.h"
#include "Game/ThrowingGameGameState.h"

// Sets default values for this component's properties
UTargetObjectPoolComponent::UTargetObjectPoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTargetObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTargetObjectPoolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

ABaseTarget* UTargetObjectPoolComponent::GetABaseTarget()
{
	ABaseTarget* newTarget = nullptr;

	for (ABaseTarget* target : m_BaseTargetPool)
	{
		if (!target->GetIsActive())
		{
			newTarget = target;
			break;
		}
	}
	
	if (newTarget == nullptr)
	{
		if (BP_BaseTarget != nullptr)
		{
			FVector SpawnLocation = FVector(0, 0, 0);
			FRotator SpawnRot = FRotator(0, 0, 0);
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			newTarget = GetWorld()->SpawnActor<ABaseTarget>(BP_BaseTarget, SpawnLocation, SpawnRot, SpawnParams);

			AThrowingGameGameState* GameState = Cast<AThrowingGameGameState>(GetWorld()->GetGameState());
			if (GameState)
			{
				newTarget->GetReceivePointsHandler().AddDynamic(GameState, &AThrowingGameGameState::FReceivePointsHandler);
			}
			else
			{
				//error
			}

			m_BaseTargetPool.Add(newTarget);
		}
	}

	newTarget->Reset();
	return newTarget;
}

ASplineTarget* UTargetObjectPoolComponent::GetASplineTarget()
{
	ASplineTarget* newTarget = nullptr;

	for (ASplineTarget* target : m_SplineTargetPool)
	{
		if (!target->GetIsActive())
		{
			newTarget = target;
			break;
		}
	}

	if (newTarget == nullptr)
	{
		if (BP_SplineTarget != nullptr)
		{
			FVector SpawnLocation = FVector(0, 0, 0);
			FRotator SpawnRot = FRotator(0, 0, 0);
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			newTarget = GetWorld()->SpawnActor<ASplineTarget>(BP_SplineTarget, SpawnLocation, SpawnRot, SpawnParams);

			AThrowingGameGameState* GameState = Cast<AThrowingGameGameState>(GetWorld()->GetGameState());
			if (GameState)
			{
				newTarget->GetReceivePointsHandler().AddDynamic(GameState, &AThrowingGameGameState::FReceivePointsHandler);
			}
			else
			{
				//error
			}

			m_SplineTargetPool.Add(newTarget);
		}
	}

	return newTarget;
}

ASideToSideTarget* UTargetObjectPoolComponent::GetASideToSideTarget()
{
	ASideToSideTarget* newTarget = nullptr;

	for (ASideToSideTarget* target : m_SideToSideTargetPool)
	{
		if (!target->GetIsActive())
		{
			newTarget = target;
			break;
		}
	}

	if (newTarget == nullptr)
	{
		if (BP_SplineTarget != nullptr)
		{
			FVector SpawnLocation = FVector(0, 0, 0);
			FRotator SpawnRot = FRotator(0, 0, 0);
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			newTarget = GetWorld()->SpawnActor<ASideToSideTarget>(BP_SideToSideTarget, SpawnLocation, SpawnRot, SpawnParams);

			AThrowingGameGameState* GameState = Cast<AThrowingGameGameState>(GetWorld()->GetGameState());
			if (GameState)
			{
				newTarget->GetReceivePointsHandler().AddDynamic(GameState, &AThrowingGameGameState::FReceivePointsHandler);
			}
			else
			{
				//error
			}

			m_SideToSideTargetPool.Add(newTarget);
		}
	}

	return newTarget;
}

