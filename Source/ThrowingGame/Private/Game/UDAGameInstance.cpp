// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/UDAGameInstance.h"
#include "HAL/PlatformTime.h"
#include "HAL/ThreadHeartBeat.h"
#include "Containers/Ticker.h"

UUDAGameInstance::UUDAGameInstance()
{
    TimerMax = 300.0f;

    GameArea = EGameArea::MAINMENU;
}

void UUDAGameInstance::Init()
{
    Super::Init();

    TickDelegateHandle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UUDAGameInstance::Tick));

    LoadMenu();
}

void UUDAGameInstance::Shutdown()
{
    Super::Shutdown();

    FTSTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
}

void UUDAGameInstance::LoadMenu()
{
    GameArea = EGameArea::MAINMENU;

    EnterLevel(); //TODO remove this
}

void UUDAGameInstance::EnterLevel()
{
    GameArea = EGameArea::START;

    Timer = TimerMax;

    StartLevel();
}

void UUDAGameInstance::StartLevel()
{
    GameArea = EGameArea::INGAME;
}

void UUDAGameInstance::EndLevel()
{
    GameArea = EGameArea::SCOREBOARD;

    //do things

    LoadScoreBoard();
}

void UUDAGameInstance::LoadScoreBoard()
{

}

bool UUDAGameInstance::Tick(float DeltaTime)
{
    if (GameArea == EGameArea::INGAME)
    {
        Timer -= DeltaTime;

        if (Timer <= 0)
        {
            EndLevel();
        }
    }

	return true;
}

FString UUDAGameInstance::GetTimerAsClock()
{
    int32 totalSeconds = FMath::FloorToInt(Timer);

    int32 hours = totalSeconds / 3600;
    int32 minutes = (totalSeconds % 3600) / 60;
    int32 seconds = totalSeconds % 60;

    if (hours > 0)
    {
        return FString::Printf(TEXT("%02d:%02d:%02d"), hours, minutes, seconds);
    }
    else
    {
        return FString::Printf(TEXT("%02d:%02d"), minutes, seconds);
    }
}
