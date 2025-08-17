// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ThrowingGameGameState.generated.h"

class AMissionHub;
class UTargetObjectPoolComponent;

UCLASS()
class THROWINGGAME_API AThrowingGameGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AThrowingGameGameState();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Game State Properties")
	AMissionHub* m_MissionHub;

	UPROPERTY(EditDefaultsOnly, Category = "Game State Properties")
	UTargetObjectPoolComponent* m_TargetPool;

	UPROPERTY(EditDefaultsOnly, Category = "Game State Properties")
	int32 m_Score;

public:
	virtual void Tick(float DeltaTime) override;

	void StartLevel();

	void EndLevel();

	AMissionHub* GetMissionHub();

	UTargetObjectPoolComponent* GetTargetPool();

	UFUNCTION()
	void FReceivePointsHandler(int32 points, int32 pointsOG, float distanceVal, float airTimeStyle, float speedVal);
};
