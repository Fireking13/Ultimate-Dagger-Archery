// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMissionComplete);

UCLASS()
class THROWINGGAME_API AMissionBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissionBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool isNotDone;

	class ABaseTarget* m_Target;

	UFUNCTION()
	void FOnTargetDeactivationHandler();
	
	UPROPERTY(BlueprintAssignable, Category = "Mission Properties");
	FOnMissionComplete OnMissionComplete;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartUp(TArray<FVector> points);

	FOnMissionComplete& GetMissionHandler() { return OnMissionComplete; };
};
