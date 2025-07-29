// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetObjectPoolComponent.generated.h"

class ABaseTarget;
class ASplineTarget;
class ASideToSideTarget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THROWINGGAME_API UTargetObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetObjectPoolComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	TArray<ABaseTarget*> m_BaseTargetPool;

	TArray<ASplineTarget*> m_SplineTargetPool;

	TArray<ASideToSideTarget*> m_SideToSideTargetPool;

	UPROPERTY(EditDefaultsOnly, Category = "Target Pool Properties")
	TSubclassOf<ABaseTarget> BP_BaseTarget;

	UPROPERTY(EditDefaultsOnly, Category = "Target Pool Properties")
	TSubclassOf<ASplineTarget> BP_SplineTarget;

	UPROPERTY(EditDefaultsOnly, Category = "Target Pool Properties")
	TSubclassOf<ASideToSideTarget> BP_SideToSideTarget;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	ABaseTarget* GetABaseTarget();

	ASplineTarget* GetASplineTarget();

	ASideToSideTarget* GetASideToSideTarget();
};
