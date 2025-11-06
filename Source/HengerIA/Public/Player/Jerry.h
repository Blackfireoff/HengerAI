// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HengerIAGameMode.h"
#include "GameFramework/Character.h"
#include "Jerry.generated.h"

class UTeamDataAsset;
enum class ETeamID : uint8;

UCLASS()
class HENGERIA_API AJerry : public ACharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    class UAIPerceptionStimuliSourceComponent* StimuliSource;

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* SK_Gun;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* ShootMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DamageMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float CurrentHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float WeaponDamage = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Team")
	UTeamDataAsset* TeamDataAsset;

public:
	// Sets default values for this character's properties
	AJerry();

	UPROPERTY(EditDefaultsOnly, Category = "Data")
	ETeamID Team = ETeamID::Team_Purple;

	virtual void TakeDamage(float DamageAmount);

	UFUNCTION()
	void ApplyTeamAsset(ETeamID TeamToAssigned);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ShootInput();

	virtual void Die();

};
