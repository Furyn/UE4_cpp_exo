// Copyright Epic Games, Inc. All Rights Reserved.
#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Green,text)

#include "ThirdPersonCPPGameMode.h"
#include "ThirdPersonCPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

AThirdPersonCPPGameMode::AThirdPersonCPPGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AThirdPersonCPPGameMode::OnPlayerKilled(AController* Controller)
{
	if(IsValid(Controller))
	{
		Controller->UnPossess();
		RestartPlayer(Controller);
	}
}
