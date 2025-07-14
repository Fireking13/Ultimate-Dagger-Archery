// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldActors/Interactable/Targets/BaseTarget.h"
#include "Projectile/Dagger.h"

// Sets default values
ABaseTarget::ABaseTarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	StaticMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));
	StaticMeshComponent->SetupAttachment(RootComponent);

	HitAngle = -0.9f;

	Tags.Add(TEXT("Target"));

}

// Called when the game starts or when spawned
void ABaseTarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseTarget::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*if (OtherActor->IsA(ADagger::StaticClass())) // 
	{

	}*/
}

void ABaseTarget::HitCheck(AActor* dagger)
{
	FVector forward = GetActorForwardVector();
	FVector daggerForward = dagger->GetActorForwardVector();

	float dot = FVector::DotProduct(daggerForward, forward);

	if (dot < HitAngle)
	{
		SendPoints(1); //TODO: remove place holder
	}
	else
	{
		SendPoints(0);
	}
}

void ABaseTarget::SendPoints(int32 num)
{
}

