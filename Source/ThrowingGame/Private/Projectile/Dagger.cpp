// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile/Dagger.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/ThrowingGameCharacter.h"
#include "PlacedDagger.h"
#include "Math/UnrealMathUtility.h"

ADagger::ADagger()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetNotifyRigidBodyCollision(true);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComponent->SetGenerateOverlapEvents(false);
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

	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ADagger::OnOverlapBegin);
	//CapsuleComponent->OnComponentHit.AddDynamic(this, &ADagger::OnHit);

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

	SpinSpeed = 0.f;
	MaxSpinSpeed = 1000.0f;
	MinSpinSpeed = 100.0f;
	SpinDir = 1;

	InWall = false;
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

			//SetActorRelativeRotation();
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
{/*
	if (IsActive)
	{
		if (OtherActor->ActorHasTag("Projectile") || OtherActor->ActorHasTag("Projectile Never Hit"))
		{
			return;
		}

		DestroyProjectile();

		if (BP_PlacedDagger != nullptr)
		{
			//FVector hitLocation = SweepResult.ImpactPoint;
			//SetActorLocation(hitLocation);

			FVector SpawnLocation = Hit.ImpactPoint;
			FRotator SpawnRot = GetActorRotation();
			FActorSpawnParameters SpawnParams;

			GetWorld()->SpawnActor<APlacedDagger>(BP_PlacedDagger, SpawnLocation, SpawnRot, SpawnParams);
		}
	}*/
}

//FVector hitLocation = SweepResult.ImpactPoint;
			//SetActorLocation(hitLocation);

void ADagger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
 	if (IsActive)
	{
		if (OtherActor->ActorHasTag("Projectile") || OtherActor->ActorHasTag("Projectile Never Hit"))
		{
			return;
		}

		DestroyProjectile();

		if (BP_PlacedDagger != nullptr)
		{
			FVector ClosestPoint;
			OtherComp->GetClosestPointOnCollision(GetActorLocation(), ClosestPoint);
			FVector SpawnLocation = ClosestPoint;

			//FVector SpawnLocation = SweepResult.ImpactPoint; //find where it hit the wall
			FRotator SpawnRot = GetActorRotation();
			FActorSpawnParameters SpawnParams;

			APlacedDagger* placedDagger = GetWorld()->SpawnActor<APlacedDagger>(BP_PlacedDagger, SpawnLocation, SpawnRot, SpawnParams);
			placedDagger->AdjustHitBox();
		}
	}
}

void ADagger::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ADagger::InitializeStats()
{
}

void ADagger::Shoot()
{
	HasShoot = true;

	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Ignore);

	SpinSpeed = FMath::FRandRange(MinSpinSpeed, MaxSpinSpeed);

	int8 num = FMath::RandRange(0, 1);

	SpinDir = (num > 0) ? 1 : -1;
}

void ADagger::FoucedAdjust(FVector targetPos)
{
	if (IsActive)
	{
		ResetSpawnLocations();

		SetActorLocation(SpawnLocations[LocationIndex]);

		FVector direction = (targetPos - GetActorLocation()).GetSafeNormal();
		SetActorRotation(direction.Rotation());
	}
}

void ADagger::Adjust(FVector targetPos)
{
	if (IsActive)
	{
		FVector direction = (targetPos - GetActorLocation()).GetSafeNormal();

		FRotator newRotation = FMath::RInterpTo(GetActorRotation(), direction.Rotation(), GetWorld()->GetDeltaSeconds(), SlerpSpeed);

		SetActorRotation(newRotation);
	}
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

		StaticMeshComponent->SetVisibility(true);
		StaticMeshComponent->SetHiddenInGame(false);
	}
}

void ADagger::DestroyProjectile()
{
	IsActive = false;
	HasShoot = false;
	InWall = false;

	CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	StaticMeshComponent->SetVisibility(false);
	StaticMeshComponent->SetHiddenInGame(true);

	ProjectileMovementComponent->Velocity = FVector::ZeroVector;
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
