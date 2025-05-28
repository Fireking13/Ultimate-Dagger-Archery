// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile/Dagger.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/ThrowingGameCharacter.h"
#include "PlacedDagger.h"

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
	else
	{
		ResetSpawnLocations();


	}
}

void ADagger::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

void ADagger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ADagger::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ADagger::InitializeStats()
{
}

void ADagger::Shoot()
{
	FoucedAdjust();
	HasShoot = true;
}

void ADagger::FoucedAdjust()
{
	ResetSpawnLocations();


}

void ADagger::Adjust(FVector targetPos)
{
	if (true)
	{

	}
}

void ADagger::Spawn(AThrowingGameCharacter* player)
{
	PlayerCharacter = player;
}

void ADagger::Reset(int posNum)
{
	ResetSpawnLocations();

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

void ADagger::ResetSpawnLocations()
{
	if (PlayerCharacter)
	{
		FVector pos = PlayerCharacter->GetFirstPersonCameraComponent()->GetComponentLocation();
		FVector rightVec = PlayerCharacter->GetFirstPersonCameraComponent()->GetRightVector();

		SpawnLocations.Add(pos + (-rightVec) * 100.0f);
		SpawnLocations.Add(pos + FVector(0.0f, 0.0f, 59.04f) + (-rightVec) * 64.0f);
		SpawnLocations.Add(pos + FVector(0.0f, 0.0f, 100.0f));
		SpawnLocations.Add(pos + FVector(0.0f, 0.0f, 59.04f) + rightVec * 64.0f);
		SpawnLocations.Add(pos + rightVec * 100.0f);
	}
}
