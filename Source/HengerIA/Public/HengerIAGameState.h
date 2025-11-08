#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Player/DA/TeamDataAsset.h" // Pour ETeamID
#include "HengerIAGameState.generated.h"

UCLASS()
class HENGERIA_API AHengerIAGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AHengerIAGameState();

	void UpdateTeamScores();

	UPROPERTY(Transient, Replicated, BlueprintReadOnly, Category = "Score")
	int32 TeamPurpleKills;

	UPROPERTY(Transient, Replicated, BlueprintReadOnly, Category = "Score")
	int32 TeamGreenKills;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};