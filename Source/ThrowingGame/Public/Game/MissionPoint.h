// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionPoint.generated.h"

UCLASS()
class THROWINGGAME_API AMissionPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissionPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool isNotDone;

	//UPROPERTY(EditDefaultsOnly, Category = "ArtifactMissionClass")
	//TSubclassOf<class AArtifactMission> GroundMission;

	//UPROPERTY(EditDefaultsOnly, Category = "FixingMissionClass")
	//TSubclassOf<class AFixingMission> AirMission;

	//UPROPERTY(EditDefaultsOnly, Category = "FixingMissionClass")
	//TSubclassOf<class AFixingMission> MovingSplineMission;

	//UPROPERTY(EditDefaultsOnly, Category = "FixingMissionClass")
	//TSubclassOf<class AFixingMission> MovingSideToSideMission;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void startUp(FVector pos);
};
