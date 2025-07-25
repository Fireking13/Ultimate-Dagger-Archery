// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionPoint.generated.h"

UENUM(BlueprintType)
enum class EMissionTypes : uint8
{
	GROUND	UMETA(DisplayName = "Ground"),
	AIR		UMETA(DisplayName = "Air"),
	SPLINE	UMETA(DisplayName = "MovingSpline"),
	SIDE	UMETA(DisplayName = "MovingSideToSide")
};


UCLASS()
class THROWINGGAME_API AMissionPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissionPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool IsActive;

	class AMissionBase* m_Mission;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mission Properties")
	EMissionTypes m_MissionType;

	UPROPERTY(EditDefaultsOnly, Category = "Mission Properties")
	TSubclassOf<class AMissionGround> GroundMission;

	UPROPERTY(EditDefaultsOnly, Category = "Mission Properties")
	TSubclassOf<class AMissionAir> AirMission;

	UPROPERTY(EditDefaultsOnly, Category = "Mission Properties")
	TSubclassOf<class AMissionSpline> MovingSplineMission;

	UPROPERTY(EditDefaultsOnly, Category = "Mission Properties")
	TSubclassOf<class AMissionSideToSide> MovingSideToSideMission;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartUp();

	bool GetIsActive();
};
