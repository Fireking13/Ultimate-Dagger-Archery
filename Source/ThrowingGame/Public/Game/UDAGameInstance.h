// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UDAGameInstance.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EGameArea : uint8
{
	MAINMENU	UMETA(DisplayName = "MainMenu"),
	START		UMETA(DisplayName = "StartOfTheGame"),
	INGAME		UMETA(DisplayName = "InGame"),
	SCOREBOARD	UMETA(DisplayName = "ScoreBoard")
};

UCLASS()
class THROWINGGAME_API UUDAGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UUDAGameInstance();

	virtual void Init() override;

	virtual void Shutdown() override;

	void LoadMenu();

	void EnterLevel();

	void StartLevel();

	void EndLevel();

	void LoadScoreBoard();

protected:
	bool Tick(float DeltaTime);

	FTSTicker::FDelegateHandle TickDelegateHandle;

	float Timer;

	float TimerMax;

	EGameArea GameArea;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class AThrowingGameCharacter* PlayerCharacterRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class AThrowingGameGameMode* GameModeRef;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class AThrowingGameGameState* GameStateRef;

public:

	UFUNCTION(BlueprintCallable)
	float GetTimer() { return Timer; }

	UFUNCTION(BlueprintCallable)
	FString GetTimerAsClock();

	UFUNCTION(BlueprintCallable)
	EGameArea GetGameArea() { return GameArea; }


protected:

public:

};
