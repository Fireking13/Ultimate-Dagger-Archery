// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionPoint.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMissionPointComplete, AMissionPoint*, Point);

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
	TSubclassOf<class AMissionGround> BP_GroundMission;

	UPROPERTY(EditDefaultsOnly, Category = "Mission Properties")
	TSubclassOf<class AMissionAir> BP_AirMission;

	UPROPERTY(EditDefaultsOnly, Category = "Mission Properties")
	TSubclassOf<class AMissionSpline> BP_SplineMission;

	UPROPERTY(EditDefaultsOnly, Category = "Mission Properties")
	TSubclassOf<class AMissionSideToSide> BP_SideToSideMission;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mission Properties")
	TArray<AActor*> Points;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mission Properties")
	float Speed = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mission Properties")
	bool FaceOutwards = true;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Mission Properties")
	AActor* ActorWithSpline;

	UPROPERTY(VisibleDefaultsOnly, Category = "EditorOnly")
	class UArrowComponent* ArrowComponent;

	UFUNCTION()
	void FOnMissionCompleteHandler();

	UPROPERTY(BlueprintAssignable, Category = "Mission Properties");
	FOnMissionPointComplete OnMissionPointComplete;

	TArray<FVector> PointLocs;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void StartUp();

	bool GetIsActive();

	FOnMissionPointComplete& GetMissionHandler() { return OnMissionPointComplete; };
};
