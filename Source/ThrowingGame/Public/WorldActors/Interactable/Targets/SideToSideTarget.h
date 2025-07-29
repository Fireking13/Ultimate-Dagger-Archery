// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WorldActors/Interactable/Targets/BaseTarget.h"
#include "SideToSideTarget.generated.h"

/**
 * 
 */
UCLASS()
class THROWINGGAME_API ASideToSideTarget : public ABaseTarget
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASideToSideTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	float Speed;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	//FVector StartPoint;

	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	//FVector EndPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	TArray<FVector> Points;

private:
	int Index;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Spawn(FVector loc, FRotator rot) override;

	virtual void Reset() override;

	virtual void Deactivate() override;

	//void SetUpPoints(FVector start, FVector end);

	void SetUpPoints(TArray<FVector> points);
};
