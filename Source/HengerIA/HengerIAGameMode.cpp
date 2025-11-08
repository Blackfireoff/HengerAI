// Copyright Epic Games, Inc. All Rights Reserved.

#include "HengerIAGameMode.h"

#include "HengerIAGameState.h"
#include "Player/JerryPlayer.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Player/JerryPlayerState.h"

AHengerIAGameMode::AHengerIAGameMode()
{
	// stub
}

void AHengerIAGameMode::OnPlayerDied(AController* PlayerController)
{
	if (!PlayerController)
	{
		return;
	}

	APawn* DeadPawn = PlayerController->GetPawn();
    
	if (DeadPawn)
	{
		PlayerController->UnPossess(); 
		FTimerHandle RespawnHandle;
        
		FTimerDelegate RespawnDelegate = FTimerDelegate::CreateUObject(
			this, 
			&AHengerIAGameMode::DelayedRestartPlayer, 
			PlayerController,
			DeadPawn 
		);

		GetWorldTimerManager().SetTimer(
			RespawnHandle,
			RespawnDelegate,
			RespawnDelay,
			false
		);
	}
    
	UE_LOG(LogTemp, Warning, TEXT("Player died, respawn in %f seconds..."), RespawnDelay);
}

void AHengerIAGameMode::ReportKill(AController* KillerController, AController* VictimController)
{
	if (AJerryPlayerState* VictimPS = VictimController ? VictimController->GetPlayerState<AJerryPlayerState>() : nullptr)
	{
		VictimPS->AddDeath();
	}

	if (AJerryPlayerState* KillerPS = KillerController ? KillerController->GetPlayerState<AJerryPlayerState>() : nullptr)
	{
		if (KillerController != VictimController)
		{
			KillerPS->AddKill();
		}
	}

	if (AHengerIAGameState* HGameState = GetGameState<AHengerIAGameState>())
	{
		HGameState->UpdateTeamScores();
	}
}

void AHengerIAGameMode::DelayedRestartPlayer(AController* Controller, APawn* DeadPawn)
{
	if (DeadPawn)
	{
		DeadPawn->Destroy();
	}
    
	if (Controller)
	{
		RestartPlayer(Controller);
	}
}
