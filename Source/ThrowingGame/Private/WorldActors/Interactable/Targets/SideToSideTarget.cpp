// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldActors/Interactable/Targets/SideToSideTarget.h"

ASideToSideTarget::ASideToSideTarget()
{
	PrimaryActorTick.bCanEverTick = true;

	Speed = 100.0f;
	Index = 0;
}

void ASideToSideTarget::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(Destroy_TimerHandle, this, &ASideToSideTarget::Deactivate, LifeSpan, false);
}

void ASideToSideTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsActive && Points.Num() >= 2)
	{
		FVector thisPoint = Points[Index];

		FVector NewLocation = FMath::VInterpConstantTo(GetActorLocation(), thisPoint, DeltaTime, Speed);
		SetActorLocation(NewLocation);

		if (FVector::Dist(NewLocation, thisPoint) < 1.0f)
		{
			Index = (Index + 1) % Points.Num();
		}
	}
}

void ASideToSideTarget::Spawn(FVector loc, FRotator rot)
{
	Reset();

	SetActorLocationAndRotation(loc, rot);

	GetWorldTimerManager().SetTimer(Destroy_TimerHandle, this, &ASideToSideTarget::Deactivate, LifeSpan, false);
}

void ASideToSideTarget::Reset()
{
	IsActive = true;
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));

	StaticMeshComponent->SetVisibility(true);
	StaticMeshComponent->SetHiddenInGame(false);

	StaticMeshComponent->SetCastShadow(true);
	StaticMeshComponent->bCastDynamicShadow = true;
	StaticMeshComponent->bCastStaticShadow = true;

	Index = 0;

	Health = MaxHealth;
}

void ASideToSideTarget::Deactivate()
{
	RemoveDaggers();

	IsActive = false;

	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	StaticMeshComponent->SetVisibility(false);
	StaticMeshComponent->SetHiddenInGame(true);

	StaticMeshComponent->SetCastShadow(false);
	StaticMeshComponent->bCastDynamicShadow = false;
	StaticMeshComponent->bCastStaticShadow = false;

	GetWorld()->GetTimerManager().ClearTimer(Destroy_TimerHandle);

	OnTargetDeactivation.Broadcast();
	//OnTargetDeactivation.Clear();
}

void ASideToSideTarget::SetUpPoints(TArray<FVector> points)
{
	Points = points;
}

/*void ASideToSideTarget::SetUpPoints(FVector start, FVector end)
{
	StartPoint = start;
	EndPoint = end;
}*/
