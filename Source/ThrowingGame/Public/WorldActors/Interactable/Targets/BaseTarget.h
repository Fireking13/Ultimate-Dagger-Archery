// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTarget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetDeactivation);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(
	FReceivePointsHandler,
	int32, points,
	int32, pointsOG,
	float, distanceVal,
	float, airTimeStyle,
	float, speedVal
);

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
	int32 MaxHealth;

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

	UPROPERTY(BlueprintAssignable, Category = "Target Properties")
	FReceivePointsHandler ReceivePoints;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	bool NotCentered;

	float DistanceIntervalVal;
	float DistanceIntervalDis;
	float DistanceMin;
	float DistanceValMax;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Target Properties")
	float Speed;

protected:

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere)
	class USphereComponent* TestSphereComponent;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Spawn(FVector loc, FRotator rot);

	virtual void Reset();

	void Deactivate();

	void HitCheck(AActor* dagger, FVector HitPoint);

	void CalculatePoints(float dis, class ADagger* dagger);

	void SendPoints(float num);

	bool GetIsActive();

	FOnTargetDeactivation& GetTargetHandler() { return OnTargetDeactivation; };

	FReceivePointsHandler& GetReceivePointsHandler() { return ReceivePoints; };

	void RemoveDaggers();

	void SetSpeed(float speed) { Speed = speed; }
	float GetSpeed() { return Speed; }

protected:
	void LifeSpanCheck(float deltaTime);
};
