// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile/Dagger.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/ThrowingGameCharacter.h"

ADagger::ADagger()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	//SphereComponent->SetCollisionProfileName("HurtBox");
	SphereComponent->SetNotifyRigidBodyCollision(true);


	SphereComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); // Set Visibility channel to Block
	//SphereComponent->SetCollisionProfileName("OverlapAllDynamic");
	SphereComponent->SetGenerateOverlapEvents(true);

	SphereComponent->SetSimulatePhysics(false);
	SphereComponent->SetEnableGravity(false);

	SphereComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	SphereComponent->CanCharacterStepUpOn = ECB_No;

	RootComponent = SphereComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	StaticMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//StaticMeshComponent->SetCollisionProfileName("HurtBox");
	StaticMeshComponent->SetupAttachment(SphereComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(SphereComponent);
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;


	IsActive = false;
	HasShoot = false;
}

void ADagger::BeginPlay()
{
	Super::BeginPlay();
}

void ADagger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasShoot)
	{



		ProjectileMovementComponent->Velocity = GetActorForwardVector() * Speed * DeltaTime * 100.0f;
	}
}

void ADagger::Shoot()
{
	Adjust();
	HasShoot = true;
}

void ADagger::Adjust()
{
	if (true)
	{

	}
}

void ADagger::Spawn(AThrowingGameCharacter* player)
{
	PlayerCharacter = player;

	SpawnLocations.Add(PlayerCharacter->GetActorLocation() + (-PlayerCharacter->GetActorRightVector()) * 100.0f);
	SpawnLocations.Add(PlayerCharacter->GetActorLocation() + FVector(0.0f, 0.0f, 59.04f) + (-PlayerCharacter->GetActorRightVector()) * 64.0f);
	SpawnLocations.Add(PlayerCharacter->GetActorLocation() + FVector(0.0f, 0.0f, 100.0f));
	SpawnLocations.Add(PlayerCharacter->GetActorLocation() + FVector(0.0f, 0.0f, 59.04f) + PlayerCharacter->GetActorRightVector() * 64.0f);
	SpawnLocations.Add(PlayerCharacter->GetActorLocation() + PlayerCharacter->GetActorRightVector() * 100.0f);
}

void ADagger::Reset(int posNum)
{
	IsActive = true;
	LocationIndex = posNum;
	SetActorLocation(PlayerCharacter->GetActorLocation());
	SetActorRotation(FRotator(0.0f, PlayerCharacter->GetControlRotation().Yaw, 0.0f));
}

void ADagger::DestroyProjectile()
{
	IsActive = false;
	HasShoot = false;
}
