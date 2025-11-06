// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/JerryController.h"

#include "EnhancedInputSubsystems.h"

class UEnhancedInputLocalPlayerSubsystem;

void AJerryController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
