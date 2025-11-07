// Copyright Epic Games, Inc. All Rights Reserved.

#include "HengerIAGameMode.h"

#include "Player/JerryPlayer.h"
#include "Engine/World.h"
#include "TimerManager.h"

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
