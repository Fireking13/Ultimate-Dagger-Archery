// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/MissionBase.h"
#include "MissionSpline.generated.h"

/**
 * 
 */
UCLASS()
class THROWINGGAME_API AMissionSpline : public AMissionBase
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	AMissionSpline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mission Properties")
	TArray<FVector> Points;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mission Properties")
	AActor* ActorWithSpline;

	class USplineComponent* SplineComp;

	float DistanceAlongSpline;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartUp(TArray<FVector> points, AActor* actorWithSpline, float speed, bool faceOutwards) override;
};
