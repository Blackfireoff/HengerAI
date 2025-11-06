// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/UI/LifeBarWidget.h"

void ULifeBarWidget::SetLifePercentage(float Percent)
{
	HealthBar->SetPercent(Percent);
}
