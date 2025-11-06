// Fill out your copyright notice in the Description page of Project Settings.


#include "Spawner/AISpawner.h"

#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Player/DA/TeamDataAsset.h"

// Sets default values
AAISpawner::AAISpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	SpawnCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Spawn Capsule"));
	SpawnCapsule->SetupAttachment(RootComponent);

	SpawnCapsule->SetRelativeLocation(FVector(0.0f, 0.0f, 90.0f));
	SpawnCapsule->SetCapsuleSize(35.0f, 90.0f);
	SpawnCapsule->SetCollisionProfileName(FName("NoCollision"));

	SpawnDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn Direction"));
	SpawnDirection->SetupAttachment(RootComponent);

	SpawnMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Spawn Mesh"));
	SpawnMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AAISpawner::BeginPlay()
{
	Super::BeginPlay();

	if (TeamDataAsset)
	{
		if (FTeamData* TeamData = TeamDataAsset->TeamAssets.Find(TeamToAssign))
		{
			SpawnMesh->SetMaterial(0, TeamData->SpawnerBaseMaterial);
		}
	}

	SpawnAI();
}

// Called every frame
void AAISpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAISpawner::SpawnAI()
{
	if (IsValid(EnemyClass))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		AJerryAI* SpawnedEnemy = GetWorld()->SpawnActor<AJerryAI>(EnemyClass, SpawnCapsule->GetComponentTransform(), SpawnParams);
		
		if (SpawnedEnemy)
		{
			SpawnedEnemy->OnAIDied.AddDynamic(this, &AAISpawner::OnAIDied);
			SpawnedEnemy->ApplyTeamAsset(TeamToAssign);
		}
	}
}

void AAISpawner::OnAIDied()
{
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &AAISpawner::SpawnAI, RespawnDelay);
}

