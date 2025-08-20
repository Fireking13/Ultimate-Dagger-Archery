// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ThrowingGameGameState.generated.h"

class AMissionHub;
class UTargetObjectPoolComponent;
class UPointDisplayComponent;

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
	UPointDisplayComponent* m_PointDisplay;

	UPROPERTY(EditDefaultsOnly, Category = "Game State Properties")
	int32 m_Score;

	bool DoScoreEffect;
	float ScoreSizeMult;
	float ScoreSizeMax;
	float ScoreEffectTime;
	float ScoreEffectTimer;

public:
	virtual void Tick(float DeltaTime) override;

	void StartLevel();

	void EndLevel();

	AMissionHub* GetMissionHub() { return m_MissionHub; }

	UTargetObjectPoolComponent* GetTargetPool() { return m_TargetPool; }

	UFUNCTION(BlueprintCallable)
	UPointDisplayComponent* GetPointDisplay() { return m_PointDisplay; }

	UFUNCTION()
	void FReceivePointsHandler(int32 points, int32 pointsOG, float distanceVal, float airTimeStyle, float speedVal);

	UFUNCTION()
	void FPointsEffectHandler(float effectTime);

	UFUNCTION(BlueprintCallable)
	int32 GetScore() { return m_Score; }

	UFUNCTION(BlueprintCallable)
	float GetScoreSizeMult() { return ScoreSizeMult; }

	void CalScoreSize(float DeltaTime);
};
