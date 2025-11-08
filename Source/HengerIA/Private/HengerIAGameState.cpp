#include "HengerIAGameState.h"

#include "HengerIAGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Player/JerryPlayerState.h" // Important pour caster

AHengerIAGameState::AHengerIAGameState()
{
	TeamPurpleKills = 0;
	TeamGreenKills = 0;
}

void AHengerIAGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHengerIAGameState, TeamPurpleKills);
	DOREPLIFETIME(AHengerIAGameState, TeamGreenKills);
}

void AHengerIAGameState::UpdateTeamScores()
{
	if (!HasAuthority())
	{
		return;
	}

	int32 NewPurpleKills = 0;
	int32 NewGreenKills = 0;

	for (APlayerState* PS : PlayerArray)
	{
		if (AJerryPlayerState* JerryPS = Cast<AJerryPlayerState>(PS))
		{
			if (JerryPS->GetTeam() == ETeamID::Team_Purple)
			{
				NewPurpleKills += JerryPS->GetKills();
			}
			else if (JerryPS->GetTeam() == ETeamID::Team_Green)
			{
				NewGreenKills += JerryPS->GetKills();
			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Updated Team Scores - Purple: %d, Green: %d"), NewPurpleKills, NewGreenKills);

	TeamPurpleKills = NewPurpleKills;
	TeamGreenKills = NewGreenKills;
}