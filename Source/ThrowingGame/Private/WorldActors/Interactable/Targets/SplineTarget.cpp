// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldActors/Interactable/Targets/SplineTarget.h"

ASplineTarget::ASplineTarget()
{
	PrimaryActorTick.bCanEverTick = true;

	//Speed = 100.0f;
	NotCentered = false;
}

void ASplineTarget::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(Destroy_TimerHandle, this, &ASplineTarget::Deactivate, LifeSpan, false);
}

void ASplineTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASplineTarget::Spawn(FVector loc, FRotator rot)
{
	Reset();

	SetActorLocationAndRotation(loc, rot);

	GetWorldTimerManager().SetTimer(Destroy_TimerHandle, this, &ASplineTarget::Deactivate, LifeSpan, false);
}

void ASplineTarget::Reset()
{
	IsActive = true;
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));

	StaticMeshComponent->SetVisibility(true);
	StaticMeshComponent->SetHiddenInGame(false);

	StaticMeshComponent->SetCastShadow(true);
	StaticMeshComponent->bCastDynamicShadow = true;
	StaticMeshComponent->bCastStaticShadow = true;

	Health = MaxHealth;
}


