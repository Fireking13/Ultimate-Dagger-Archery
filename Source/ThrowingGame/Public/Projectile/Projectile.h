// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class THROWINGGAME_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile Properties")
	float Speed; //Optional movement code

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile Properties")
	float Damage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile Properties")
	float LifeSpan;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile Properties")
	FVector CurrentVelocity; //Optional movement code

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Projectile Properties")
	FTimerHandle Destroy_TimerHandle;

protected:

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMeshComponent; 

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent; 

	UPROPERTY()
	class AThrowingGameGameMode* GameMode;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit);

	UFUNCTION()
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "Projectile Functions")
	float GetProjectileDamage();

	UFUNCTION(BlueprintCallable, Category = "Projectile Functions")
	float GetProjectileSpeed();

	UFUNCTION(BlueprintCallable, Category = "Projectile Functions")
	float GetProjectileLifeSpan();

	UFUNCTION(BlueprintCallable, Category = "Projectile Functions")
	class UProjectileMovementComponent* GetProjectileMovement();

	void SetDamage(float damage);

	void SetSpeed(float speed);

	UFUNCTION()
	virtual void DestroyProjectile();

	UFUNCTION()
	virtual void InitializeStats();
};
