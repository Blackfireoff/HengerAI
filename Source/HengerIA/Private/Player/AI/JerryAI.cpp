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

	GetMesh()->SetCollisionProfileName(FName("BlockAll"));

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

	HealthWidget->SetVisibility(ESlateVisibility::Hidden);
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	GetWorldTimerManager().SetTimer(RespawnTimer, this, &AJerryAI::Respawn, DespawnTime, false);
}

void AJerryAI::Respawn()
{
	OnAIDied.Broadcast();
	Destroy();
}

void AJerryAI::TakeDamage(float DamageAmount)
{
	Super::TakeDamage(DamageAmount);

	HealthWidget->SetLifePercentage(CurrentHealth / MaxHealth);
}
