// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "JerryHUD.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHUDReady);

class ULifeBarWidget;
/**
 * 
 */
UCLASS()
class HENGERIA_API AJerryHUD : public AHUD
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUserWidget> CrossWidgetClass;

	UPROPERTY()
	UUserWidget* CrossWidget;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<ULifeBarWidget> LifeBarWidgetClass;

	UPROPERTY()
	ULifeBarWidget* LifeBarWidget;

	UFUNCTION()
	void OnPawnChanged(APawn* OldPawn, APawn* NewPawn);
	
	void BindToPawnDelegates(APawn* Pawn);

	UFUNCTION()
	void ShowCrossWidget();

	UFUNCTION()
	void HideCrossWidget();

	UFUNCTION()
	void ShowLifeBarWidget();

	UFUNCTION()
	void HideLifeBarWidget();

	UFUNCTION()
	void UpdateLifeBar(float NewLifePercentage);

public:
	FOnHUDReady OnHUDReady;
};
