// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TeamDataAsset.generated.h"


enum class ETeamID : uint8;

USTRUCT(BlueprintType)
struct FTeamData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team Asset Data")
	UMaterialInterface* Quinn01_Material;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team Asset Data")
	UMaterialInterface* Quinn02_Material;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team Asset Data")
	UMaterialInterface* SpawnerBaseMaterial;
};

UCLASS()
class HENGERIA_API UTeamDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Team Assets")
	TMap<ETeamID, FTeamData> TeamAssets;
};
