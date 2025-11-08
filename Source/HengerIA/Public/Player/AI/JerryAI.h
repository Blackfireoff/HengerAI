// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Jerry.h"
#include "JerryAI.generated.h"

class ULifeBarWidget;
class UWidgetComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAIDied);

UCLASS()
class HENGERIA_API AJerryAI : public AJerry
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* HealthWidgetComponent;

	UPROPERTY(EditAnywhere, Category="Damage")
	TObjectPtr<ULifeBarWidget> HealthWidget;

	UPROPERTY(EditAnywhere, Category="Spawning")
	float DespawnTime = 3.f;

	virtual void BeginPlay() override;

	void ResetHealth();

	virtual void Die() override;

	UFUNCTION()
	void Respawn();

	FTimerHandle RespawnTimer;

public:
	AJerryAI();
	
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* TreeAsset;

	FOnAIDied OnAIDied;

	virtual void TakeDamage(float DamageAmount, AActor* DamageCauser) override;

	virtual void ApplyState(EJerryState NewJerryState) override;
};
