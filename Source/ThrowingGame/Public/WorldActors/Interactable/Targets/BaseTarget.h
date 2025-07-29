// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTarget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetDeactivation);

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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	float RingSpace;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	FVector CenterPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	float MaxPoints;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	int32 RingNum;

	UPROPERTY(BlueprintAssignable, Category = "Target Properties")
	FOnTargetDeactivation OnTargetDeactivation;

protected:

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	class USphereComponent* TestSphereComponent;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*
	UFUNCTION()
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);*/

	virtual void Spawn(FVector loc, FRotator rot);

	virtual void Reset();

	virtual void Deactivate();

	void HitCheck(AActor* dagger, FVector HitPoint);

	void CalculatePoints(float dis);

	void SendPoints(float num);

	bool GetIsActive();

	FOnTargetDeactivation& GetTargetHandler() { return OnTargetDeactivation; };

protected:
	void LifeSpanCheck(float deltaTime);
};
