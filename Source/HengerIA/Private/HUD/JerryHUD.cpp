// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/JerryHUD.h"

#include "Player/Jerry.h"
#include "Player/JerryPlayer.h"
#include "Player/UI/LifeBarWidget.h"

void AJerryHUD::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerOwner)
	{
		PlayerOwner->OnPossessedPawnChanged.AddDynamic(this, &AJerryHUD::OnPawnChanged);
		BindToPawnDelegates(PlayerOwner->GetPawn());
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

	if (LifeBarWidgetClass)
	{
		LifeBarWidget = CreateWidget<ULifeBarWidget>(GetOwningPlayerController(), LifeBarWidgetClass);
		if (LifeBarWidget)
		{
			LifeBarWidget->AddToViewport();
			LifeBarWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AJerryHUD::OnPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (AJerryPlayer* OldJerry = Cast<AJerryPlayer>(OldPawn))
	{
		OldJerry->OnAimStarted.RemoveDynamic(this, &AJerryHUD::ShowCrossWidget);
		OldJerry->OnAimEnded.RemoveDynamic(this, &AJerryHUD::HideCrossWidget);

		OldJerry->OnShowLifeBar.RemoveDynamic(this, &AJerryHUD::ShowLifeBarWidget);
		OldJerry->OnHideLifeBar.RemoveDynamic(this, &AJerryHUD::HideLifeBarWidget);
		OldJerry->OnLifeUpdated.RemoveDynamic(this, &AJerryHUD::UpdateLifeBar);
	}

	if (NewPawn)
	{
		BindToPawnDelegates(NewPawn);
	}
}

void AJerryHUD::BindToPawnDelegates(APawn* Pawn)
{
	if (AJerryPlayer* Jerry = Cast<AJerryPlayer>(Pawn))
	{
		Jerry->OnAimStarted.AddDynamic(this, &AJerryHUD::ShowCrossWidget);
		Jerry->OnAimEnded.AddDynamic(this, &AJerryHUD::HideCrossWidget);

		Jerry->OnShowLifeBar.AddDynamic(this, &AJerryHUD::ShowLifeBarWidget);
		Jerry->OnHideLifeBar.AddDynamic(this, &AJerryHUD::HideLifeBarWidget);
		Jerry->OnLifeUpdated.AddDynamic(this, &AJerryHUD::UpdateLifeBar);
		
		OnHUDReady.Broadcast();
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

void AJerryHUD::ShowLifeBarWidget()
{
	if (LifeBarWidget)
	{
		LifeBarWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void AJerryHUD::HideLifeBarWidget()
{
	LifeBarWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AJerryHUD::UpdateLifeBar(float NewLifePercentage)
{
	if (LifeBarWidget)
	{
		LifeBarWidget->SetLifePercentage(NewLifePercentage);
	}
}
