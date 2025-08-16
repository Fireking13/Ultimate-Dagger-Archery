// Copyright Epic Games, Inc. All Rights Reserved.

#include "Player/ThrowingGameCharacter.h"
#include "Projectile/Dagger.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EngineUtils.h"

#include "Projectile/Dagger.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AThrowingGameCharacter

AThrowingGameCharacter::AThrowingGameCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	MaxNumDashs = 3;
	NumDashs = MaxNumDashs;
	IsDashing = false;
	IsSliding = false;
	CanDash = true;
	DashCooldown = 0.25f;
	DashStrength = 2400.0f;
	DashRefillDelay = 3.f;
	SlideSpeed = 2400.0f;
	SlideMovementMul = 100.0f;
	SlideJumpMul = 1.75f;
	FireIndex = 1;
	AdjustTimer = 0.0f;
	AdjustTimerMax = 1.0f;
	Ammo = 0;

	AirTimeStyleDef = 1.0f;
	AirTimeStyle = AirTimeStyleDef;
	AirTimeStyleMax = 2.0f;
	AirTimeStyleIncrement = 0.1f;
	AirTimeStyleIncrementInterval = 1.0f;
	AirTimeStyleTimer = 0.0f;

	for (bool daggerSpot : DaggerSpots)
	{
		daggerSpot = false;
	}

	Tags.Add(TEXT("Projectile Never Hit"));
}

void AThrowingGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsSliding)
	{
		Sliding(DeltaTime);
	}

	SetTargetPoint();

	for (ADagger* dagger : DaggersReady)
	{
		if (dagger)
		{
			dagger->Adjust(TargetPoint);
		}
	}

	if (GetCharacterMovement()->IsFalling())
	{
		AirStyleCheck(DeltaTime);
	}
	else
	{
		AirTimeStyle = AirTimeStyleDef;
	}
}

void AThrowingGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	UCharacterMovementComponent* moveComp = GetCharacterMovement();

	OG_GroundFriction = moveComp->GroundFriction;
	OG_BrakingFrictionFactor = moveComp->BrakingFrictionFactor;
	OG_BrakingDecelerationWalking = moveComp->BrakingDecelerationWalking;

	OG_JumpHeight = moveComp->JumpZVelocity;
	OG_Speed = moveComp->MaxWalkSpeed;

	Refill();
}

void AThrowingGameCharacter::RefillDash()
{
	NumDashs++;

	if (NumDashs < MaxNumDashs)
	{
		GetWorldTimerManager().SetTimer(DashRefill_TimerHandle, this, &AThrowingGameCharacter::RefillDash, DashRefillDelay, false);
	}
}

void AThrowingGameCharacter::ResetDash()
{
	IsDashing = false;

	UCharacterMovementComponent* moveComp = GetCharacterMovement();

	moveComp->GroundFriction = OG_GroundFriction;
	moveComp->BrakingFrictionFactor = OG_BrakingFrictionFactor;
	moveComp->BrakingDecelerationWalking = OG_BrakingDecelerationWalking;
}

void AThrowingGameCharacter::Sliding(float DeltaTime)
{
	UCharacterMovementComponent* moveComp = GetCharacterMovement();

	if (moveComp->IsMovingOnGround())
	{
		if (moveComp->MaxWalkSpeed != SlideSpeed)
		{
			moveComp->MaxWalkSpeed = SlideSpeed;
		}
	}
	else
	{
		if (moveComp->MaxWalkSpeed != OG_Speed)
		{
			moveComp->MaxWalkSpeed = OG_Speed;
		}
	}

	FVector newSideDir = moveComp->Velocity.GetSafeNormal();
	if (newSideDir.IsNearlyZero())
	{
		newSideDir = SideDir; // Fallback
	}

	FHitResult HitResult;
	FVector Start = GetActorLocation();
	FVector End = Start + (newSideDir * 150.f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		QueryParams
	);

	if (bHit && !moveComp->IsWalkable(HitResult))
	{
		StopSlide();
		return;
	}

	AddMovementInput(SideDir, SlideSpeed * DeltaTime);
}

void AThrowingGameCharacter::Refill()
{
	FireIndex = 0;

	int8 index = 0;

	Ammo = MaxDaggers;

	for (bool& daggerSpot : DaggerSpots)
	{
		if (!daggerSpot)
		{
			DaggersReady[index] = GetDaggerFromPool();

			DaggersReady[index]->Reset(index);

			daggerSpot = true;
		}

		index++;
	}
}

void AThrowingGameCharacter::Shoot()
{
	if (DaggersReady[FireIndex])
	{
		DaggersReady[FireIndex]->FoucedAdjust(TargetPoint);
		DaggersReady[FireIndex]->Shoot();

		DaggersReady[FireIndex] = nullptr;

		DaggerSpots[FireIndex] = false;

		FireIndex++;
		Ammo--;

		if (FireIndex > 4)
		{
			Refill(); //TDOD: chanage
		}
	}
}

ADagger* AThrowingGameCharacter::GetDaggerFromPool()
{
	ADagger* newDagger = nullptr;

	for (ADagger* dagger : DaggerPool)
	{
		if (!dagger->GetIsActive())
		{
			newDagger = dagger;
			break;
		}
	}

	if (newDagger == nullptr)
	{
		if (BP_Dagger != nullptr)
		{
			FVector SpawnLocation = GetActorLocation();
			FRotator SpawnRot(0.0f, GetControlRotation().Yaw, 0.0f);
			FActorSpawnParameters SpawnParams;

			newDagger = GetWorld()->SpawnActor<ADagger>(BP_Dagger, SpawnLocation, SpawnRot, SpawnParams);
			DaggerPool.Add(newDagger);
			newDagger->Spawn(this);
		}
	}

	return newDagger;
}

void AThrowingGameCharacter::SetTargetPoint()
{
	FHitResult HitResult;
	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector End = Start + (FirstPersonCameraComponent->GetForwardVector() * 10000.f);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	for (TActorIterator<AActor> It(GetWorld()); It; ++It) //loop thorugh all actor
	{
		AActor* Actor = *It;

		if (Actor->ActorHasTag("Projectile") || Actor->ActorHasTag("Projectile Never Hit"))
		{
			QueryParams.AddIgnoredActor(Actor);
		}
	}

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility,
		QueryParams
	);

	if (HitResult.GetActor() != nullptr)
	{
		TargetPoint = HitResult.ImpactPoint;
	}
	else
	{
		TargetPoint = End;
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AThrowingGameCharacter::NotifyControllerChanged()
{
	Super::NotifyControllerChanged();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AThrowingGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AThrowingGameCharacter::SlideJumpCheck);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AThrowingGameCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AThrowingGameCharacter::Look);

		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &AThrowingGameCharacter::Dash);

		EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Started, this, &AThrowingGameCharacter::Slide);
		EnhancedInputComponent->BindAction(SlideAction, ETriggerEvent::Completed, this, &AThrowingGameCharacter::StopSlide);

		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Triggered, this, &AThrowingGameCharacter::Throw);

		EnhancedInputComponent->BindAction(RefillAction, ETriggerEvent::Triggered, this, &AThrowingGameCharacter::Refill);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

float AThrowingGameCharacter::GetTopSpeed()
{
	float topSpeed = FMath::Max3(SlideSpeed, DashStrength, OG_Speed);

	return topSpeed;
}

float AThrowingGameCharacter::GetDashTimer()
{
	FTimerManager& TimerManager = GetWorldTimerManager();

	float rate = TimerManager.GetTimerRate(DashRefill_TimerHandle);      
	float elapsed = TimerManager.GetTimerElapsed(DashRefill_TimerHandle);  

	if (rate <= 0.f) //safty
	{
		return 0.0f;
	}

	return FMath::Clamp(elapsed / rate, 0.0f, 1.0f);
}


void AThrowingGameCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		if (!IsSliding)
		{
			AddMovementInput(GetActorForwardVector(), MovementVector.Y);
			AddMovementInput(GetActorRightVector(), MovementVector.X);
		}
		else
		{
			AddMovementInput(GetActorRightVector(), MovementVector.X * SlideMovementMul);
		}
	}
}

void AThrowingGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AThrowingGameCharacter::Dash(const FInputActionValue& Value)
{
	if (IsDashing == false && NumDashs > 0)
	{
		IsDashing = true;
		NumDashs--;

		UCharacterMovementComponent* moveComp = GetCharacterMovement();

		moveComp->GroundFriction = 0.0f;
		moveComp->BrakingFrictionFactor = 0.0f;
		moveComp->BrakingDecelerationWalking = 0.0f;

		FVector MovementDirection = moveComp->GetLastInputVector();

		if (!MovementDirection.IsNearlyZero())
		{
			MovementDirection.Normalize();
		}
		else
		{
			MovementDirection = GetActorForwardVector(); 
		}

		moveComp->StopMovementImmediately();

		LaunchCharacter(MovementDirection * DashStrength, true, true);

		GetWorldTimerManager().SetTimer(DashCooldown_TimerHandle, this, &AThrowingGameCharacter::ResetDash, DashCooldown, false);

		if (!GetWorldTimerManager().IsTimerActive(DashRefill_TimerHandle))
		{
			GetWorldTimerManager().SetTimer(DashRefill_TimerHandle, this, &AThrowingGameCharacter::RefillDash, DashRefillDelay, false);
		}
	}
}

void AThrowingGameCharacter::Slide()
{
	IsSliding = true;
	SideDir = GetActorForwardVector();

	GetCapsuleComponent()->SetCapsuleHalfHeight(GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() / 2);
	//float newHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	UCharacterMovementComponent* moveComp = GetCharacterMovement();

	moveComp->GroundFriction = 0.0f;
	moveComp->BrakingFrictionFactor = 0.0f;
	moveComp->BrakingDecelerationWalking = 0.0f;
}

void AThrowingGameCharacter::StopSlide()
{
	if (IsSliding)
	{
		IsSliding = false;

		GetCapsuleComponent()->SetCapsuleHalfHeight(GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() * 2);
		//float newHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

		UCharacterMovementComponent* moveComp = GetCharacterMovement();

		moveComp->GroundFriction = OG_GroundFriction;
		moveComp->BrakingFrictionFactor = OG_BrakingFrictionFactor;
		moveComp->BrakingDecelerationWalking = OG_BrakingDecelerationWalking;

		moveComp->MaxWalkSpeed = OG_Speed;
	}
}

void AThrowingGameCharacter::Throw(const FInputActionValue& Value)
{
	Shoot(); //TODO FIX
}

void AThrowingGameCharacter::SlideJumpCheck()
{
	UCharacterMovementComponent* moveComp = GetCharacterMovement();

	if (IsSliding && moveComp->IsMovingOnGround())
	{
		moveComp->JumpZVelocity = OG_JumpHeight * SlideJumpMul;
	}
	else if (moveComp->JumpZVelocity != OG_JumpHeight)
	{
		moveComp->JumpZVelocity = OG_JumpHeight;
	}

	Jump();
}

void AThrowingGameCharacter::AirStyleCheck(float deltaTime)
{
	AirTimeStyleTimer += deltaTime;

	if (AirTimeStyleTimer >= AirTimeStyleIncrementInterval)
	{
		AirTimeStyleTimer = 0.0f;

		AirTimeStyle += AirTimeStyleIncrement;
		AirTimeStyle = FMath::Min(AirTimeStyle, AirTimeStyleMax);
	}
}

