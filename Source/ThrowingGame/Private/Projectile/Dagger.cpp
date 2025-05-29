// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile/Dagger.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/ThrowingGameCharacter.h"
#include "PlacedDagger.h"

ADagger::ADagger()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetNotifyRigidBodyCollision(true);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetGenerateOverlapEvents(true);
	SphereComponent->SetSimulatePhysics(false);
	SphereComponent->SetEnableGravity(false);
	SphereComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	SphereComponent->CanCharacterStepUpOn = ECB_No;

	RootComponent = SphereComponent;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetNotifyRigidBodyCollision(true);
	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	CapsuleComponent->SetGenerateOverlapEvents(true);
	CapsuleComponent->SetSimulatePhysics(false);
	CapsuleComponent->SetEnableGravity(false);
	CapsuleComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CapsuleComponent->CanCharacterStepUpOn = ECB_No;
	CapsuleComponent->SetupAttachment(SphereComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	StaticMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetupAttachment(SphereComponent);

	StaticMeshComponent->SetCastShadow(false);
	StaticMeshComponent->bCastDynamicShadow = false;
	StaticMeshComponent->bCastStaticShadow = false;

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	IsActive = false;
	HasShoot = false;

	Speed = 5000.0f;

	LerpSpeed = 20.0f;
	SlerpSpeed = 20.0f;
}

void ADagger::BeginPlay()
{
	Super::BeginPlay();
}

void ADagger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsActive)
	{
		if (HasShoot)
		{
			ProjectileMovementComponent->Velocity = GetActorForwardVector() * Speed;
		}
		else
		{
			if (PlayerCharacter != nullptr)

			{
				ResetSpawnLocations();

				FVector newLocation = FMath::VInterpTo(GetActorLocation(), SpawnLocations[LocationIndex], DeltaTime, LerpSpeed);
				SetActorLocation(newLocation);
			}
		}
	}
}

void ADagger::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!IsActive)
	{

	}
}

void ADagger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsActive)
	{

	}
}

void ADagger::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsActive)
	{

	}
}

void ADagger::InitializeStats()
{
}

void ADagger::Shoot()
{
	HasShoot = true;

	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void ADagger::FoucedAdjust(FVector targetPos)
{
	ResetSpawnLocations();

	SetActorLocation(SpawnLocations[LocationIndex]);

	FVector direction = (targetPos - GetActorLocation()).GetSafeNormal();
	SetActorRotation(direction.Rotation());
}

void ADagger::Adjust(FVector targetPos)
{
	FVector direction = (targetPos - GetActorLocation()).GetSafeNormal();

	FRotator newRotation = FMath::RInterpTo(GetActorRotation(), direction.Rotation(), GetWorld()->GetDeltaSeconds(), SlerpSpeed);

	SetActorRotation(newRotation);
}

void ADagger::Spawn(AThrowingGameCharacter* player)
{
	PlayerCharacter = player;
}

void ADagger::Reset(int posNum)
{
	if (PlayerCharacter)
	{
		ResetSpawnLocations();

		IsActive = true;
		LocationIndex = posNum;
		SetActorLocation(PlayerCharacter->GetActorLocation());
		SetActorRotation(FRotator(0.0f, PlayerCharacter->GetControlRotation().Yaw, 0.0f));

		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
}

void ADagger::DestroyProjectile()
{
	IsActive = false;
	HasShoot = false;
}

void ADagger::ResetSpawnLocations()
{
	if (PlayerCharacter != nullptr)
	{

		FVector pos = PlayerCharacter->GetFirstPersonCameraComponent()->GetComponentLocation();
		FVector forwardVec = PlayerCharacter->GetFirstPersonCameraComponent()->GetForwardVector();
		FVector rightVec = PlayerCharacter->GetFirstPersonCameraComponent()->GetRightVector();
		FVector upVec = PlayerCharacter->GetFirstPersonCameraComponent()->GetUpVector();

		SpawnLocations.Empty();

		SpawnLocations.Add(pos + upVec * -20 + (-rightVec) * 100.0f + forwardVec * 35);
		SpawnLocations.Add(pos + upVec * (59.04f - 20) + (-rightVec) * 64.0f + forwardVec * 35);
		SpawnLocations.Add(pos + upVec * (100.0f - 20) + forwardVec * 35);
		SpawnLocations.Add(pos + upVec * (59.04f - 20) + rightVec * 64.0f + forwardVec * 35);
		SpawnLocations.Add(pos + upVec * -20 + rightVec * 100.0f + forwardVec * 35);
	}
}
