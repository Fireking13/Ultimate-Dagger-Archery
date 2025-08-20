// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PointDisplayComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPointsEffectHandler, float, effectTime);

USTRUCT(BlueprintType)
struct FPointStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
    int32 FullPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
    int32 HitPoints;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
    float DistanceVal;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
    float AirTimeStyle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Points")
    float SpeedVal;

    FPointStats() = default;

    FPointStats(int32 fullPoints,int32 hitPoints, float distanceVal, float airTimeStyle, float speedVal)
        : FullPoints(fullPoints), HitPoints(hitPoints), DistanceVal(distanceVal), AirTimeStyle(airTimeStyle), SpeedVal(speedVal)
    {
    }
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THROWINGGAME_API UPointDisplayComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPointDisplayComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

    TArray<FPointStats> PopUpList;

    float PopUpTimer;
	float PopUpDelay;
    float PopUpDelayMin;
    float PopUpDelayMax;
    float PopUpDelayTemp;

    float ActiveFullPoints;
    bool ShowFull;

    int32 ActiveHitPoints;
    float ActiveDistanceVal;
    float ActiveAirTimeStyle;
    float ActiveSpeedVal;

    int32 TempPoints;

    UPROPERTY(BlueprintAssignable, Category = "Target Properties")
    FPointsEffectHandler PointsEffectHandler;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void ResetActives();
		
    UFUNCTION(BlueprintCallable)
    FString GetPopUpText(int32 index);

    void AddToList(int32 fullPoints, int32 hitPoints, float distanceVal, float airTimeStyle, float speedVal);

    void AddPoints();

    FPointsEffectHandler& GetPointsEffectHandler() { return PointsEffectHandler; };
};
