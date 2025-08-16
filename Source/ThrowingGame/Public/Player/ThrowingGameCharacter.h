// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "ThrowingGameCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class AThrowingGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SlideAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrowAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RefillAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	

public:
	AThrowingGameCharacter();

	virtual void Tick(float DeltaTime) override;

	void RefillDash();

	void ResetDash();

	void Sliding(float DeltaTime);

	void Refill();

	void Shoot();

	class ADagger* GetDaggerFromPool();

	void SetTargetPoint();

protected:
	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for Dash input */
	void Dash(const FInputActionValue& Value);

	/** Called for Sliding input */
	void Slide();

	/** Called for Sliding input */
	void StopSlide();

	/** Called for Sliding input */
	void Throw(const FInputActionValue& Value);

	void SlideJumpCheck();
	//virtual void Jump() override;
	//virtual void StopJumping() override;

	void AirStyleCheck(float deltaTime);

protected:
	FVector SideDir;

	bool IsDashing;
	bool IsSliding;
	bool CanDash;

	int32 MaxNumDashs;
	int32 NumDashs;

	float DashCooldown;
	float DashStrength;
	float DashRefillDelay;

	FTimerHandle DashCooldown_TimerHandle;
	FTimerHandle DashRefill_TimerHandle;

	float OG_GroundFriction;
	float OG_BrakingFrictionFactor;
	float OG_BrakingDecelerationWalking;

	float SlideSpeed;
	float SlideMovementMul;
	float SlideJumpMul;

	float OG_JumpHeight;
	float OG_Speed;

	static constexpr int32 MaxDaggers = 5;

	TArray<class ADagger*> DaggerPool;
	class ADagger* DaggersReady[MaxDaggers];
	bool DaggerSpots[MaxDaggers];

	int8 Ammo;
	int8 FireIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dagger")
	TSubclassOf<class ADagger> BP_Dagger;

	FVector TargetPoint;

	//FTimerHandle Adjust_TimerHandle;

	float AdjustTimer;
	float AdjustTimerMax;

	float AirTimeStyle;
	float AirTimeStyleMax;
	float AirTimeStyleIncrement;
	float AirTimeStyleIncrementInterval;
	float AirTimeStyleTimer;
	float AirTimeStyleDef;

protected:
	// APawn interface
	virtual void NotifyControllerChanged() override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	float GetAirTimeStyle() { return AirTimeStyle; }

	float GetOGSpeed() { return OG_Speed; }

	float GetTopSpeed();

	UFUNCTION(BlueprintCallable)
	int32 GetAmmo() { return Ammo; }

	UFUNCTION(BlueprintCallable)
	int32 GetNumDashes() { return NumDashs; }

	UFUNCTION(BlueprintCallable)
	float GetDashRefillDelay() { return DashRefillDelay; }

	UFUNCTION(BlueprintCallable)
	float GetDashTimer();
};

