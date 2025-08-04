// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/MissionHub.h"
#include "Game/MissionPoint.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
AMissionHub::AMissionHub()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsActive = false;

}

// Called when the game starts or when spawned
void AMissionHub::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMissionHub::FOnMissionPointCompleteHandler(AMissionPoint* point)
{
	if (IsActive)
	{
		int32 i = m_Missions.Find(point);

		if (i != INDEX_NONE)
		{
			m_MissionsActive.RemoveSingle(point);

			ReactivatePoint(i);

			AvailablePoints.Add(i);
		}
		else
		{
			// Error: Point not found in active list
		}
	}
}

void AMissionHub::CheckAvailablePoints()
{
}

// Called every frame
void AMissionHub::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMissionHub::StartUp()
{
	for (int32 i = 0; i < m_Missions.Num(); i++)
	{
		AvailablePoints.Add(i);
	}

	/*for (int32 i = AvailablePoints.Num() - 1; i > 0; i--)
	{
		int32 j = FMath::RandRange(0, i);
		AvailablePoints.Swap(i, j);
	}*/

	for (int32 i = 0; i < MaxMissionInWorld; i++)
	{
		ReactivatePoint(-1);
	}

	IsActive = true;
}

void AMissionHub::SetIsActive(bool val)
{
	IsActive = val;
}

void AMissionHub::ReactivatePoint(int32 index)
{
	int randomUnusedNum = FMath::RandRange(0, AvailablePoints.Num() - 1);
	int PointIndex = AvailablePoints[randomUnusedNum];

	m_Missions[PointIndex]->GetMissionHandler().Clear();//.RemoveDynamic(this, &AMissionHub::FOnMissionPointCompleteHandler);
	m_Missions[PointIndex]->GetMissionHandler().AddDynamic(this, &AMissionHub::FOnMissionPointCompleteHandler);
	m_Missions[PointIndex]->StartUp();

	m_MissionsActive.Add(m_Missions[PointIndex]);

	AvailablePoints.RemoveAt(randomUnusedNum);
}
