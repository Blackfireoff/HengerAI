// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Player/DA/TeamDataAsset.h"
#include "JerryPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class HENGERIA_API AJerryPlayerState : public APlayerState
{
	GENERATED_BODY()

public:	
	AJerryPlayerState();

	void SetTeam(ETeamID NewTeam);

	UFUNCTION(Category = "Team")
	ETeamID GetTeam() const { return Team; }

	// Fonctions pour les scores
	void AddKill();
	void AddDeath();

	UFUNCTION(Category = "Score")
	int32 GetKills() const { return Kills; }

	UFUNCTION(Category = "Score")
	int32 GetDeaths() const { return Deaths; }

protected:
	// Nécéssaire pour la réplication
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Transient, Replicated)
	ETeamID Team;

	UPROPERTY(Transient, Replicated)
	int32 Kills;

	UPROPERTY(Transient, Replicated)
	int32 Deaths;
};
