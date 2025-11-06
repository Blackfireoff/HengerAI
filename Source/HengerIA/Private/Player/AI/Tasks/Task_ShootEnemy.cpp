// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AI/Tasks/Task_ShootEnemy.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/AI/JerryAI.h"

EBTNodeResult::Type UTask_ShootEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UWorld* World = GetWorld();
	if (!World) return EBTNodeResult::Failed;

	

	if (AJerry* PlayerActor = Cast<AJerry>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName("TargetActor"))))
	{
		if (AJerryAI* JerryAI = Cast<AJerryAI>(OwnerComp.GetAIOwner()->GetPawn()))
		{
			FCollisionQueryParams CollisionParams;
			CollisionParams.AddIgnoredActor(JerryAI);

			
			FVector MuzzleLoc =  JerryAI->SK_Gun->GetSocketLocation(TEXT("Muzzle"));
	
			FVector MuzzleToTargetDirection = (PlayerActor->GetActorLocation() - MuzzleLoc).GetSafeNormal();
			FVector MuzzleTraceEnd = MuzzleLoc + (MuzzleToTargetDirection * JerryAI->MaxTraceDistance);

			FHitResult MuzzleHitResult;
	
			if (World->LineTraceSingleByChannel(MuzzleHitResult, MuzzleLoc, MuzzleTraceEnd, ECC_Visibility, CollisionParams))
			{

				DrawDebugLine(World, MuzzleLoc, MuzzleHitResult.ImpactPoint, FColor::Green, false, 2.0f, 0, 1.f);

				if (AActor* HitActor = MuzzleHitResult.GetActor())
				{
					if (AJerry* Jerry = Cast<AJerry>(HitActor))
					{
						if (Jerry->Team != JerryAI->Team)
						{	
							Jerry->TakeDamage(JerryAI->WeaponDamage);
							return EBTNodeResult::Succeeded;
						}
					}
				}
			}
		}

	}
	
	return EBTNodeResult::Failed;
}
