// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProjectCGameMode.h"
#include "MyProjectCCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyProjectCGameMode::AMyProjectCGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
