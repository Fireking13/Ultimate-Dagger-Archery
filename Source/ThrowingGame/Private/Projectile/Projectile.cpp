// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile/Projectile.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

float AProjectile::GetProjectileDamage()
{
	return Damage;
}

float AProjectile::GetProjectileSpeed()
{
	return 0.0f;
}

float AProjectile::GetProjectileLifeSpan()
{
	return 0.0f;
}

UProjectileMovementComponent* AProjectile::GetProjectileMovement()
{
	return nullptr;
}

void AProjectile::SetDamage(float damage)
{
}

void AProjectile::SetSpeed(float speed)
{
}

void AProjectile::DestroyProjectile()
{
}

void AProjectile::InitializeStats()
{
}
