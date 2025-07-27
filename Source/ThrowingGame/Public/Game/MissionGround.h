// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Game/MissionBase.h"
#include "MissionGround.generated.h"

/**
 * 
 */
UCLASS()
class AMissionGround : public AMissionBase
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AMissionGround();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartUp(FVector pos) override;
};
