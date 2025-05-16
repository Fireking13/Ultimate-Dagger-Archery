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

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AThrowingGameCharacter

AThrowingGameCharacter::AThrowingGameCharacter()
{
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

	NumDashs = 3;
	IsDashing = false;
	IsSliding = false;
	DashCooldown = 0.075f;
	DashStrength = 2400.0f;
	CanDash = true;
	DashRefillDelay = 3.f;
}

void AThrowingGameCharacter::RefillDash()
{
	NumDashs++;

	if (NumDashs < 3)
	{
		GetWorldTimerManager().SetTimer(DashRefill_TimerHandle, this, &AThrowingGameCharacter::RefillDash, DashRefillDelay, false);
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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AThrowingGameCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AThrowingGameCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void AThrowingGameCharacter::Move(const FInputActionValue& Value)
{
	if (IsSliding == false)
	{
		// input is a Vector2D
		FVector2D MovementVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{
			// add movement 
			AddMovementInput(GetActorForwardVector(), MovementVector.Y);
			AddMovementInput(GetActorRightVector(), MovementVector.X);
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

		FVector playerVelocity = GetVelocity();

		FVector MovementDirection = GetCharacterMovement()->GetLastInputVector();
		MovementDirection.Normalize();

		GetCharacterMovement()->StopMovementImmediately();

		LaunchCharacter(MovementDirection * DashStrength, true, true);

		if (!GetWorldTimerManager().IsTimerActive(DashRefill_TimerHandle))
		{
			GetWorldTimerManager().SetTimer(DashRefill_TimerHandle, this, &AThrowingGameCharacter::RefillDash, DashRefillDelay, false);
		}
	}
}

void AThrowingGameCharacter::Slide(const FInputActionValue& Value)
{
	SideDir = GetActorForwardVector();
}

void AThrowingGameCharacter::Throw(const FInputActionValue& Value)
{

}
