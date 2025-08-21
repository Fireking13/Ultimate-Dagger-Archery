// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MissionSpline.h"
#include "WorldActors/Interactable/Targets/SplineTarget.h"
#include "Game/ThrowingGameGameState.h"
#include "TargetObjectPoolComponent.h"
#include "Components/SplineComponent.h"

AMissionSpline::AMissionSpline()
{
	PrimaryActorTick.bCanEverTick = true;

	DistanceAlongSpline = 0.f;
}

void AMissionSpline::BeginPlay()
{
	Super::BeginPlay();
}

void AMissionSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsActive && SplineComp)
	{
		DistanceAlongSpline += Speed * DeltaTime;

		//DistanceAlongSpline = FMath::Clamp(DistanceAlongSpline, 0.0f, SplineComp->GetSplineLength());
		DistanceAlongSpline = FMath::Fmod(DistanceAlongSpline, SplineComp->GetSplineLength());  //TODO more logic on if its closed and if it should go back wards

		if (DistanceAlongSpline < 0)
		{
			DistanceAlongSpline += SplineComp->GetSplineLength(); //safty also test it more TODO
		}

		FVector newLoc = SplineComp->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
		FRotator newRot = SplineComp->GetRotationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
		m_Target->SetActorLocationAndRotation(newLoc, newRot);
	}
}

void AMissionSpline::StartUp(TArray<FVector> points, AActor* actorWithSpline, float speed, bool faceOutwards)
{
	DistanceAlongSpline = 0.f;

	Speed = speed;
	FaceOutwards = faceOutwards;
	ActorWithSpline = actorWithSpline;

	FVector spawnLoc = GetActorLocation();

	SplineComp = ActorWithSpline->FindComponentByClass<USplineComponent>();
	if (SplineComp)
	{
		spawnLoc = SplineComp->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
	}

	AGameStateBase* baseGameState = GetWorld()->GetGameState();

	AThrowingGameGameState* gameState = Cast<AThrowingGameGameState>(baseGameState);

	if (gameState)
	{
		ASplineTarget* target = gameState->GetTargetPool()->GetASplineTarget();

		target->GetTargetHandler().Clear();//.RemoveDynamic(this, &AMissionSpline::FOnTargetDeactivationHandler);
		target->GetTargetHandler().AddDynamic(this, &AMissionSpline::FOnTargetDeactivationHandler);
		target->Spawn(spawnLoc, GetActorRotation());

		//TODO flip mesh and make sure it is reset when its decatvated

		m_Target = target;
	}

	IsActive = true;
}
