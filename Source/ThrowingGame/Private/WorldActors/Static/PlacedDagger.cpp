// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldActors/Static/PlacedDagger.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Player/ThrowingGameCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlacedDagger::APlacedDagger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	StaticMeshComponent->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	StaticMeshComponent->SetupAttachment(SphereComponent);
	
	/*BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetNotifyRigidBodyCollision(true);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Block);
	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->SetSimulatePhysics(false);
	BoxComponent->SetEnableGravity(false);
	BoxComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Default, 1.f));
	BoxComponent->CanCharacterStepUpOn = ECB_Yes;
	BoxComponent->SetupAttachment(SphereComponent); */

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxComponent->SetCollisionObjectType(ECC_WorldStatic);

	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); 

	BoxComponent->SetGenerateOverlapEvents(true);
	BoxComponent->CanCharacterStepUpOn = ECB_Yes;
	BoxComponent->SetupAttachment(SphereComponent);

	HightAdjNum = 75.f;
	
	Tags.Add(TEXT("Projectile Never Hit"));
	Tags.Add(TEXT("Target Never Hit"));
}

// Called when the game starts or when spawned
void APlacedDagger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlacedDagger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HitWall)
	{
		if (ACharacter* Player = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
		{
			FVector PlayerLocation = Player->GetActorLocation();
			FVector PlatformLocation = BoxComponent->GetComponentLocation();

			if (PlayerLocation.Z > PlatformLocation.Z + HightAdjNum)
			{
				BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
			}
			else
			{
				BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
			}
		}
	}
}

void APlacedDagger::AdjustHitBox(FRotator meshRot, bool hitWall)
{
	FRotator currentRot = BoxComponent->GetRelativeRotation();

	FRotator correctedRot = FRotator(0.f, currentRot.Yaw, currentRot.Roll);

	BoxComponent->SetRelativeRotation(correctedRot);

	StaticMeshComponent->SetWorldRotation(meshRot);

	HitWall = hitWall;
}

