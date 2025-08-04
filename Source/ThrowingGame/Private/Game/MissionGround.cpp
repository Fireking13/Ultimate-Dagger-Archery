// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/MissionGround.h"
#include "WorldActors/Interactable/Targets/BaseTarget.h"
#include "Game/ThrowingGameGameState.h"
#include "TargetObjectPoolComponent.h"

AMissionGround::AMissionGround()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMissionGround::BeginPlay()
{
	Super::BeginPlay();
}

void AMissionGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMissionGround::StartUp(TArray<FVector> points)
{
	AGameStateBase* baseGameState = GetWorld()->GetGameState();

	AThrowingGameGameState* gameState = Cast<AThrowingGameGameState>(baseGameState);

	if (gameState)
	{
		m_Target = gameState->GetTargetPool()->GetABaseTarget();
	}

	FVector newPos;

	FVector start = GetActorLocation();
	FVector end = start + (FVector(0, 0, -1) * 1000.0f);

	TArray<FHitResult> hitResults;
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	bool bHit = GetWorld()->LineTraceMultiByObjectType(hitResults, start, end, ObjectQueryParams);

	for (FHitResult& hit : hitResults)
	{
		AActor* hitActor = hit.GetActor();

		if (hitActor->ActorHasTag("Projectile") || hitActor->ActorHasTag("Projectile Never Hit") || hitActor->ActorHasTag("Target"))
		{
			continue;
		}

		newPos = hit.Location;
		break;
	}

	if (newPos == FVector::ZeroVector)
	{
		newPos = end;
	}

	m_Target->GetTargetHandler().Clear();//.RemoveDynamic(this, &AMissionGround::FOnTargetDeactivationHandler);
	m_Target->GetTargetHandler().AddDynamic(this, &AMissionGround::FOnTargetDeactivationHandler);
	m_Target->Spawn(newPos, GetActorRotation());
}
