// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/Projectile.h"
#include "Dagger.generated.h"

/**
 * 
 */
UCLASS()
class THROWINGGAME_API ADagger : public AProjectile
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ADagger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool HasShoot;

	//FTimerHandle Adjust_TimerHandle;

	class AThrowingGameCharacter* PlayerCharacter;

	TArray<FVector> SpawnLocations;

	int8 LocationIndex;

	float LerpSpeed;
	
	float SlerpSpeed;

	float SpinSpeed;
	float MaxSpinSpeed;
	float MinSpinSpeed;
	int8 SpinDir;

	bool InWall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dagger")
	TSubclassOf<class APlacedDagger> BP_PlacedDagger;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnHit(UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit) override;

	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	virtual void OnOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult) override;

	virtual void InitializeStats() override;

	void Shoot();

	void FoucedAdjust(FVector targetPos);

	void Adjust(FVector targetPos);

	void Spawn(class AThrowingGameCharacter* player);

	void Reset(int posNum);

	virtual void DestroyProjectile() override;

	void ResetSpawnLocations();
};
