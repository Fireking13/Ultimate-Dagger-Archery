// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTarget.generated.h"

UCLASS()
class THROWINGGAME_API ABaseTarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	float LifeSpan;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	FTimerHandle Destroy_TimerHandle;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	bool IsActive;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	int32 Health;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	float HitAngle;

protected:

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	//void Spawn();

	//void Reset();

	void HitCheck(AActor* dagger);

	void SendPoints(int32 num);
};
