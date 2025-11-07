// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HengerIAGameMode.generated.h"

/**
 *  Simple GameMode for a first person game
 */

UENUM(BlueprintType)
enum class ETeamID : uint8
{
	Team_Purple	UMETA(DisplayName = "Purple Team"),
	Team_Green	UMETA(DisplayName = "Green Team"),
};


UCLASS(abstract)
class AHengerIAGameMode : public AGameModeBase
{

	GENERATED_BODY()

public:
	AHengerIAGameMode();

	void OnPlayerDied(AController* PlayerController);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Game Flow")
	float RespawnDelay = 3.0f;
    
	UFUNCTION()
	void DelayedRestartPlayer(AController* Controller, APawn* DeadPawn);
	
};



