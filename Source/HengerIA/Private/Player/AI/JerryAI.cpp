// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AI/JerryAI.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Player/AI/JerryAIController.h"
#include "Player/UI/LifeBarWidget.h"

AJerryAI::AJerryAI()
{
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidget"));
	HealthWidgetComponent->SetupAttachment(RootComponent);
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	HealthWidgetComponent->SetDrawSize(FVector2D(150.f, 20.f));

	GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);
	
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Ignore);

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void AJerryAI::BeginPlay()
{
	Super::BeginPlay();
	
	HealthWidget = Cast<ULifeBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
	ResetHealth();
}

void AJerryAI::ResetHealth()
{
	CurrentHealth = MaxHealth;

	HealthWidget->SetLifePercentage(1.0f);
	HealthWidget->SetVisibility(ESlateVisibility::Visible);
}

void AJerryAI::Die()
{
	Super::Die();

	ApplyState(EJerryState::Dead);
	HealthWidget->SetVisibility(ESlateVisibility::Hidden);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &AJerryAI::Respawn, DespawnTime, false);
}

void AJerryAI::Respawn()
{
	OnAIDied.Broadcast();
	Destroy();
}

void AJerryAI::TakeDamage(float DamageAmount, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageCauser);

	HealthWidget->SetLifePercentage(CurrentHealth / MaxHealth);

	// Si l'IA est déjà morte, ne faites rien
	if (CurrentHealth <= 0.f)
	{
		return;
	}
	
	if (DamageCauser)
	{
		if (AJerry* Attacker = Cast<AJerry>(DamageCauser))
		{
			if (Attacker->Team != this->Team)
			{
				if (AJerryAIController* AIController = Cast<AJerryAIController>(GetController()))
				{
					if (UBlackboardComponent* BB = AIController->BlackboardComponent)
					{
						BB->SetValueAsObject(FName("TargetActor"), Attacker);
						BB->SetValueAsBool(FName("CanSeeEnemy"), true);
					}
				}
			}
		}
	}
}

void AJerryAI::ApplyState(EJerryState NewJerryState)
{
	Super::ApplyState(NewJerryState);

	if (AJerryAIController* AIController = Cast<AJerryAIController>(GetController()))
	{
		if (UBlackboardComponent* BB = AIController->BlackboardComponent)
		{
			if (NewJerryState == EJerryState::Alive)
			{
				BB->SetValueAsBool(FName("IsDead"), false);
				return;
			}
			if (NewJerryState == EJerryState::Dead)
			{
				BB->SetValueAsBool(FName("IsDead"), true);
			}
		}
	}
}
