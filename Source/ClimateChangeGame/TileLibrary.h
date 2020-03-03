// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "TileLibrary.generated.h"

USTRUCT(BlueprintType)
struct FTileData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UMaterialInterface* Material;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float BaseHeight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float BaseHeightVariance;
};

USTRUCT(BlueprintType)
struct FTile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CurrentHeight;
	UPROPERTY()
		int32 InstanceId;
};

/**
 * 
 */
UCLASS()
class CLIMATECHANGEGAME_API UTileLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
