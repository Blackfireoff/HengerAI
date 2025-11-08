#include "Player/JerryPlayerState.h"

#include "HengerIAGameMode.h"
#include "Net/UnrealNetwork.h"

AJerryPlayerState::AJerryPlayerState()
{
	Kills = 0;
	Deaths = 0;
	Team = ETeamID::Team_Purple; // Équipe par défaut
}

// Définition de la réplication
void AJerryPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AJerryPlayerState, Team);
	DOREPLIFETIME(AJerryPlayerState, Kills);
	DOREPLIFETIME(AJerryPlayerState, Deaths);
}

void AJerryPlayerState::SetTeam(ETeamID NewTeam)
{
	// Seul le serveur peut changer l'équipe
	if (HasAuthority())
	{
		Team = NewTeam;
	}
}

void AJerryPlayerState::AddKill()
{
	if (HasAuthority())
	{
		Kills++;
	}
}

void AJerryPlayerState::AddDeath()
{
	if (HasAuthority())
	{
		Deaths++;
	}
}