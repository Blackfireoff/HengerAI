// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "JerryHUD.generated.h"

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

	UFUNCTION()
	void ShowCrossWidget();

	UFUNCTION()
	void HideCrossWidget();
};
