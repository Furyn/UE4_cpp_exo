// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ThirdPersonCPPGameMode.generated.h"

UCLASS(minimalapi)
class AThirdPersonCPPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AThirdPersonCPPGameMode();
	UFUNCTION()
	void OnPlayerKilled(AController* Controller);
};



