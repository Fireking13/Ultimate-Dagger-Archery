// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionHub.generated.h"

class AMissionPoint;

UCLASS()
class THROWINGGAME_API AMissionHub : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissionHub();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "MissionHub", meta = (EditCondition = "Point Locations"))
	TArray<AMissionPoint*> m_Missions;

	TArray<AMissionPoint*> m_MissionsActive;

	UPROPERTY(EditInstanceOnly, Category = "MissionHub", meta = (EditCondition = "Point Locations"))
	TArray<AActor*> PointsLoc;

	UPROPERTY(EditInstanceOnly, Category = "MissionHub", meta = (EditCondition = "Max amount of missions active in the world"))
	int32 MaxMissionInWorld;

	int32 NumActiveMissions;

	bool IsActive;

	TArray<int32> AvailablePoints;

	UFUNCTION()
	void FOnMissionPointCompleteHandler();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartUp();

	void SetIsActive(bool val);

	//void PointDone();

	void ReactivatePoint();
};
