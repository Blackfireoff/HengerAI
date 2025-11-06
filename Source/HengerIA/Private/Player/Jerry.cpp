// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Jerry.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "Player/DA/TeamDataAsset.h"

// Sets default values
AJerry::AJerry()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	SK_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));

	StimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	StimuliSource->RegisterForSense(UAISense_Sight::StaticClass());
}

// Called when the game starts or when spawned
void AJerry::BeginPlay()
{
	Super::BeginPlay();

	const FAttachmentTransformRules AttachmentRule(EAttachmentRule::SnapToTarget, false);
	SK_Gun->AttachToComponent(GetMesh(), AttachmentRule, TEXT("HandGrip_R"));
}

void AJerry::ShootInput()
{
	// Shoot animation
	if (ShootMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(ShootMontage);
		}
	}
}

void AJerry::Die()
{
}

void AJerry::TakeDamage(float DamageAmount)
{
	if (CurrentHealth <= 0.f)
	{
		return;
	}

	CurrentHealth -= DamageAmount;

	if (CurrentHealth <= 0.f)
	{
		Die();
		return;
	}
	
	// Damage animation
	if (DamageMontage)
	{
		if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
		{
			AnimInstance->Montage_Play(DamageMontage);
		}
	}
	
}

void AJerry::ApplyTeamAsset(ETeamID TeamToAssigned)
{
	Team = TeamToAssigned;
	if (TeamDataAsset)
	{
		if (FTeamData* TeamData = TeamDataAsset->TeamAssets.Find(Team))
		{
			GetMesh()->SetMaterial(0, TeamData->Quinn01_Material);
			GetMesh()->SetMaterial(1, TeamData->Quinn02_Material);
		}
	}
}




