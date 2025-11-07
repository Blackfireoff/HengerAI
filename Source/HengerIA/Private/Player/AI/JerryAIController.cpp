// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AI/JerryAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Player/AI/JerryAI.h"

AJerryAIController::AJerryAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Component"));

	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
    SetPerceptionComponent(*AIPerceptionComponent);
	
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightConfig->PeripheralVisionAngleDegrees = 90.0f;
	SightConfig->SightRadius = 3000.0f;
	SightConfig->SetMaxAge(8.0f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 200.0f;
	SightConfig->LoseSightRadius = 3500;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	AIPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void AJerryAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AJerryAI* JerryAI = Cast<AJerryAI>(InPawn))
	{
		if (JerryAI->TreeAsset && JerryAI->TreeAsset->BlackboardAsset){
			BlackboardComponent->InitializeBlackboard(*JerryAI->TreeAsset->BlackboardAsset);
			BehaviorTreeComponent->StartTree(*JerryAI->TreeAsset);
		}
	}
}

void AJerryAIController::BeginPlay()
{
	Super::BeginPlay();

	if (AIPerceptionComponent)
	{
		AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AJerryAIController::OnTargetPerceptionUpdated);
	}
}

void AJerryAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus const Stimulus)
{
	if (AJerry* Jerry = Cast<AJerry>(Actor))
	{
		ETeamID JerryTeam = Jerry->Team;
		if (AJerryAI* SelfJerryAI = Cast<AJerryAI>(GetPawn()))
		{
			if (SelfJerryAI->Team == JerryTeam) return;
		}

		if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
		{
			if (Stimulus.WasSuccessfullySensed() && Jerry->GetJerryState() == EJerryState::Alive)
			{
				
				BlackboardComponent->SetValueAsBool("CanSeeEnemy", true);
				BlackboardComponent->SetValueAsObject("TargetActor", Actor);
			}
			else
			{
				BlackboardComponent->SetValueAsBool("CanSeeEnemy", false);
				BlackboardComponent->SetValueAsObject("TargetActor", nullptr);
			}
		}
	}
}
