// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldActors/Interactable/Targets/BaseTarget.h"
#include "MovingTarget.generated.h"

/**
 * 
 */
UCLASS()
class THROWINGGAME_API AMovingTarget : public ABaseTarget
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMovingTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	int32 InbetweenPointsNum;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	class USplineComponent* SplinePath;

	/*
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	TArray<FVector> MainTrailPoints;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	TArray<FVector> PathPoints;*/

private:
	//FVector TargetPathPoint;

	//int32 Index;

	float DistanceAlongSpline;

protected:
	//void MakeFullPath();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//void SetMainTrailPoints(TArray<FVector> mainTrailPoints);

	void SetUpSpline(TArray<FVector> Points);
};
