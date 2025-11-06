// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/JerryHUD.h"

#include "Player/Jerry.h"
#include "Player/JerryPlayer.h"

void AJerryHUD::BeginPlay()
{
	Super::BeginPlay();

	// Bind to delegate
	if (PlayerOwner)
	{
		if (AJerryPlayer* Jerry = Cast<AJerryPlayer>(PlayerOwner->GetPawn()))
		{
			Jerry->OnAimStarted.AddDynamic(this, &AJerryHUD::ShowCrossWidget);
			Jerry->OnAimEnded.AddDynamic(this, &AJerryHUD::HideCrossWidget);
		}
	}

	// Initialize widgets
	if (CrossWidgetClass)
	{
		CrossWidget = CreateWidget<UUserWidget>(GetOwningPlayerController(), CrossWidgetClass);
		if (CrossWidget)
		{
			CrossWidget->AddToViewport();
			CrossWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AJerryHUD::ShowCrossWidget()
{
	CrossWidget->SetVisibility(ESlateVisibility::Visible);
}

void AJerryHUD::HideCrossWidget()
{
	CrossWidget->SetVisibility(ESlateVisibility::Hidden);
}
