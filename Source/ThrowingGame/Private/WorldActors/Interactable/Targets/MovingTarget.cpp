// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldActors/Interactable/Targets/MovingTarget.h"
#include "Components/SplineComponent.h"

AMovingTarget::AMovingTarget()
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

void AMovingTarget::BeginPlay()
{
	Super::BeginPlay();
}

void AMovingTarget::Tick(float DeltaTime)
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

void AMovingTarget::SetUpSpline(TArray<FVector> Points)
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

void AMovingTarget::MakeFullPath()
{
	//make PathPoints

	TargetPathPoint = PathPoints[Index];
}

void AMovingTarget::Tick(float DeltaTime)
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

void AMovingTarget::SetMainTrailPoints(TArray<FVector> mainTrailPoints)
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
