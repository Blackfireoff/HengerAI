// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Player/AI/JerryAI.h"
#include "AISpawner.generated.h"

class UArrowComponent;
class UCapsuleComponent;

UCLASS()
class HENGERIA_API AAISpawner : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* SpawnCapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UMeshComponent* SpawnMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* SpawnDirection;
	
public:	
	// Sets default values for this actor's properties
	AAISpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy Spawner")
	TSubclassOf<AJerryAI> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy Spawner")
	float RespawnDelay = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy Spawner")
	ETeamID TeamToAssign = ETeamID::Team_Purple;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Enemy Spawner")
	UTeamDataAsset* TeamDataAsset;

	FTimerHandle SpawnTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnAI();

	UFUNCTION()
	void OnAIDied();

};
