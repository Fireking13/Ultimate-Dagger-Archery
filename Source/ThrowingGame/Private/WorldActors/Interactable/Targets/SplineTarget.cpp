// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldActors/Interactable/Targets/SplineTarget.h"
#include "Components/SplineComponent.h"

ASplineTarget::ASplineTarget()
{
	PrimaryActorTick.bCanEverTick = true;

	Speed = 100.0f;
	InbetweenPointsNum = 50; 
	//TargetPathPoint = FVector::ZeroVector;
	//Index = 0;

	DistanceAlongSpline = 0.0f;

	SplinePath = CreateDefaultSubobject<USplineComponent>(TEXT("SplinePath"));
	SplinePath->SetupAttachment(RootComponent);
}

void ASplineTarget::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(Destroy_TimerHandle, this, &ASplineTarget::Deactivate, LifeSpan, false);
}

void ASplineTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsActive && SplinePath && SplinePath->GetNumberOfSplinePoints() > 1)
	{
		DistanceAlongSpline += Speed * DeltaTime;

		float splineLength = SplinePath->GetSplineLength();

		if (DistanceAlongSpline > splineLength)
		{
			DistanceAlongSpline = 0.0f; 
		}

		FVector NewLocation = SplinePath->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
		FRotator NewRotation = SplinePath->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);

		NewRotation.Pitch = 0.0f;
		NewRotation.Roll = 0.0f;

		SetActorLocationAndRotation(NewLocation, NewRotation);
	}
}

void ASplineTarget::Spawn(FVector loc, FRotator rot)
{
	Reset();

	SetActorLocation(loc);
	StaticMeshComponent->SetRelativeRotation(rot);

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
}

void ASplineTarget::Deactivate()
{
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
	OnTargetDeactivation.Clear();
}

void ASplineTarget::SetUpSpline(TArray<FVector> Points)
{
	if (Points.Num() >= 2)
	{
		SplinePath->ClearSplinePoints();

		for (int32 i = 0; i < Points.Num(); ++i)
		{
			SplinePath->AddSplinePoint(Points[i], ESplineCoordinateSpace::World, false);
			SplinePath->SetSplinePointType(i, ESplinePointType::Curve, false); //<- test TODO
		}

		DistanceAlongSpline = 0.0f;

		SplinePath->UpdateSpline();
	}
}

/*

void ASplineTarget::MakeFullPath()
{
	//make PathPoints

	TargetPathPoint = PathPoints[Index];
}

void ASplineTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsActive)
	{
		if (TargetPathPoint != FVector::ZeroVector)
		{
			if (GetActorLocation() == TargetPathPoint)
			{
				Index++;

				if (Index > PathPoints.Num() - 1)
				{
					Index = 0;
				}

				TargetPathPoint = PathPoints[Index];
			}

			FVector direction = (TargetPathPoint - GetActorLocation()).GetSafeNormal();
			FVector currentVelocity = direction * Speed;
			FVector newLocation = GetActorLocation() + (currentVelocity * DeltaTime);

			FRotator newRotation = direction.Rotation();
			newRotation.Pitch = 0.0f;
			newRotation.Roll = 0.0f;

			//SetActorLocation(NewLocation, true);
			SetActorLocationAndRotation(newLocation, newRotation, true);
		}
	}
}

void ASplineTarget::SetMainTrailPoints(TArray<FVector> mainTrailPoints)
{
	if (mainTrailPoints.Num() >= 3)
	{
		MainTrailPoints = mainTrailPoints;
		MakeFullPath();
	}
	else
	{
		//error
	}
}*/
