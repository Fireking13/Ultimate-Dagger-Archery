// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/PointDisplayComponent.h"


// Sets default values for this component's properties
UPointDisplayComponent::UPointDisplayComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    ActiveFullPoints = 0;
    ActiveHitPoints = 0;
    ActiveDistanceVal = 0.f;
    ActiveAirTimeStyle = 0.f;
    ActiveSpeedVal = 0.f;
    TempPoints = 0;

    PopUpTimer = 0.f;
    PopUpDelayMax = 2.f;
    PopUpDelayMin = 0.5f;
    PopUpDelay = PopUpDelayMax;
}

// Called when the game starts
void UPointDisplayComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UPointDisplayComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (PopUpList.Num())
    {
        if (PopUpList.Num() == 1)
        {
            PopUpDelay = PopUpDelayMax;
        }
        else
        {
            //shrink PopUpDelay
        }

        if (PopUpTimer <= 0.f)
        {
            ActiveHitPoints = PopUpList[0].HitPoints;
            ActiveDistanceVal = PopUpList[0].DistanceVal;
            ActiveAirTimeStyle = PopUpList[0].AirTimeStyle;
            ActiveSpeedVal = PopUpList[0].SpeedVal;

            ActiveFullPoints = ActiveHitPoints;
            TempPoints = PopUpList[0].FullPoints;

            PopUpList.RemoveAt(0);

            PopUpTimer = PopUpDelay;

            if (ShowFull)
            {
                PointsEffectHandler.Broadcast((PopUpDelayTemp / 2));

                ActiveFullPoints = 0;
            }

            ShowFull = false; //safty
        }
    }

    if (PopUpTimer > 0.f)
    {
        PopUpTimer -= DeltaTime;

        if (PopUpTimer < PopUpDelay / 2.f)
        {
            AddPoints();
        }
    }
    else if(ShowFull)
    {
        PointsEffectHandler.Broadcast((PopUpDelayTemp / 2));

        ActiveFullPoints = 0;

        ShowFull = false;
    }
}

void UPointDisplayComponent::ResetActives()
{
	ActiveHitPoints = 0;
	ActiveDistanceVal = 0.f;
	ActiveAirTimeStyle = 0.f;
	ActiveSpeedVal = 0.f;

    //TODO think more on this
}

FString UPointDisplayComponent::GetPopUpText(int32 index)
{
    switch (index)
    {
    case 0:
        if (ActiveFullPoints > 0 || ActiveHitPoints > 0)
        {
            return FString::Printf(TEXT("+%d Points"), ShowFull ? ActiveFullPoints : ActiveHitPoints);
        }
    case 1:
        if (ActiveDistanceVal > 0.f)
        {
            return FString::Printf(TEXT("+%.1f Distance"), ActiveDistanceVal);
        }
        [[fallthrough]];
    case 2:
        if (ActiveAirTimeStyle > 1.f)
        {
            return FString::Printf(TEXT("+%.1f Air Time"), ActiveAirTimeStyle);
        }
        [[fallthrough]];
    case 3:
        if (ActiveSpeedVal > 1.f)
        {
            return FString::Printf(TEXT("+%.1f Speed"), ActiveSpeedVal);
        }
        break;
    default:
        break;
    }

    return FString{};
}

void UPointDisplayComponent::AddToList(int32 fullPoints, int32 hitPoints, float distanceVal, float airTimeStyle, float speedVal)
{
    PopUpList.Add(FPointStats(fullPoints, hitPoints, distanceVal, airTimeStyle, speedVal));
}

void UPointDisplayComponent::AddPoints()
{
    ShowFull = true;

    ActiveFullPoints = TempPoints;
    TempPoints = 0;

    PopUpDelayTemp = PopUpDelay;

    ResetActives();
}

