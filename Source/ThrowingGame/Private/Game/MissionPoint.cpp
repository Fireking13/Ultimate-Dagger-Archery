// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/MissionPoint.h"
#include "Game/MissionAir.h"
#include "Game/MissionGround.h"
#include "Game/MissionSpline.h"
#include "Game/MissionSideToSide.h"

// Sets default values
AMissionPoint::AMissionPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_Mission = nullptr;

	IsActive = false;
}

// Called when the game starts or when spawned
void AMissionPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMissionPoint::FOnMissionCompleteHandler()
{
	IsActive = false;

	OnMissionPointComplete.Broadcast(this);
	//OnMissionPointComplete.Clear();
}

// Called every frame
void AMissionPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMissionPoint::StartUp()
{
	if (m_Mission == nullptr)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (m_MissionType == EMissionTypes::AIR)
		{
			m_Mission = GetWorld()->SpawnActor<AMissionAir>(BP_AirMission, GetActorLocation(), GetActorRotation(), SpawnParams);
		}
		else if (m_MissionType == EMissionTypes::GROUND)
		{
			m_Mission = GetWorld()->SpawnActor<AMissionGround>(BP_GroundMission, GetActorLocation(), GetActorRotation(), SpawnParams);
		}
		else if (m_MissionType == EMissionTypes::SIDE)
		{
			m_Mission = GetWorld()->SpawnActor<AMissionSideToSide>(BP_SideToSideMission, GetActorLocation(), GetActorRotation(), SpawnParams);
		}
		else if (m_MissionType == EMissionTypes::SPLINE)
		{
			m_Mission = GetWorld()->SpawnActor<AMissionSpline>(BP_SplineMission, GetActorLocation(), GetActorRotation(), SpawnParams);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Mission type not selected in mission point"));
		}
	}

	if (m_Mission)
	{
		m_Mission->GetMissionHandler().Clear();//RemoveDynamic(this, &AMissionPoint::FOnMissionCompleteHandler);
		m_Mission->GetMissionHandler().AddDynamic(this, &AMissionPoint::FOnMissionCompleteHandler);
		m_Mission->StartUp(Points);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Mission did not spawn in mission point"));
	}
}

bool AMissionPoint::GetIsActive()
{
	return IsActive;
}
