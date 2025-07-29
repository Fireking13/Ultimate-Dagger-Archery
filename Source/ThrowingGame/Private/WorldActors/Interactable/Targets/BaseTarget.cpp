// Fill out your copyright notice in the Description page of Project Settings.

#include "WorldActors/Interactable/Targets/BaseTarget.h"
#include "Projectile/Dagger.h"
#include "Components/SphereComponent.h"
#include "Math/UnrealMathUtility.h"

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


	/*TestSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("TestSphereComponent"));
	TestSphereComponent->SetNotifyRigidBodyCollision(true);
	TestSphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TestSphereComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	TestSphereComponent->SetGenerateOverlapEvents(false);
	TestSphereComponent->SetSimulatePhysics(false);
	TestSphereComponent->SetEnableGravity(false);
	TestSphereComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	TestSphereComponent->CanCharacterStepUpOn = ECB_No;*/

	IsActive = true; //TODO change
	HitAngle = -0.15f;
	RingSpace = 18.f; 
	MaxPoints = 100.f; 
	RingNum = 5;
	Health = 3;

	Tags.Add(TEXT("Target"));

}

// Called when the game starts or when spawned
void ABaseTarget::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(Destroy_TimerHandle, this, &ABaseTarget::Deactivate, LifeSpan, false);
}

// Called every frame
void ABaseTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*void ABaseTarget::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ADagger::StaticClass())) // 
	{

	}
}*/

void ABaseTarget::Spawn(FVector loc, FRotator rot)
{
	Reset();

	SetActorLocation(loc);
	StaticMeshComponent->SetRelativeRotation(rot);

	GetWorldTimerManager().SetTimer(Destroy_TimerHandle, this, &ABaseTarget::Deactivate, LifeSpan, false);
}

void ABaseTarget::Reset()
{
	IsActive = true;
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetCollisionProfileName(TEXT("BlockAll"));

	StaticMeshComponent->SetVisibility(true);
	StaticMeshComponent->SetHiddenInGame(false);

	StaticMeshComponent->SetCastShadow(true);
	StaticMeshComponent->bCastDynamicShadow = true;
	StaticMeshComponent->bCastStaticShadow = true;
}

void ABaseTarget::Deactivate()
{
	IsActive = false;

	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	StaticMeshComponent->SetVisibility(false);
	StaticMeshComponent->SetHiddenInGame(true);

	StaticMeshComponent->SetCastShadow(false);
	StaticMeshComponent->bCastDynamicShadow = false;
	StaticMeshComponent->bCastStaticShadow = false;

	GetWorld()->GetTimerManager().ClearTimer(Destroy_TimerHandle);

	OnTargetDeactivation.Broadcast();
	OnTargetDeactivation.Clear();
}

void ABaseTarget::HitCheck(AActor* dagger, FVector HitPoint)
{
	FVector OffSet = FVector(0.f, -1.0f, 97.0f);
	FVector LocalOffset = GetActorTransform().TransformVector(OffSet);
	CenterPoint = GetActorLocation() + LocalOffset;

	FVector forward = GetActorForwardVector();
	FVector daggerForward = dagger->GetActorForwardVector();

	float dot = FVector::DotProduct(daggerForward, forward);

	if (dot < HitAngle)
	{
		float dis = FVector::Dist(HitPoint, CenterPoint);

		CalculatePoints(dis);
		Health--;

		if (Health <= 0)
		{
			Deactivate();
		}
	}
	else
	{
		SendPoints(0.f);
	}
}

void ABaseTarget::CalculatePoints(float dis)
{
	float points = MaxPoints;

	for (int32 i = 0; i < RingNum; i++)
	{
		if (dis < RingSpace + (RingSpace * i) + 1)
		{
			break;
		}

		points -= MaxPoints / 4;

		points = FMath::Clamp(points, MaxPoints / 10, MaxPoints);
	}

	SendPoints(points);
}

void ABaseTarget::SendPoints(float num)
{
	UE_LOG(LogTemp, Warning, TEXT("Points = %f"), num);
}

bool ABaseTarget::GetIsActive()
{
	return IsActive;
}

void ABaseTarget::LifeSpanCheck(float deltaTime)
{

}

