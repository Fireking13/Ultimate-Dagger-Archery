// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThrowingGameGameMode.h"
#include "ThrowingGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThrowingGameGameMode::AThrowingGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
