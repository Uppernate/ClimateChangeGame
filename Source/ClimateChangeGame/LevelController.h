// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WorldTiles.h"
#include "BuildingLibrary.h"
#include "LevelController.generated.h"

class AWorldGenerator;

/**
 * 
 */
UCLASS()
class CLIMATECHANGEGAME_API ALevelController : public APlayerController
{
	GENERATED_BODY()
public:
	// Class holding tilemap and tile meshes
	UPROPERTY(BlueprintReadOnly)
		AWorldTiles* WorldTiles;
	// A class that modified the tilemap
	UPROPERTY()
		AWorldGenerator* Generator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<ERate, float> Rates;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<ERate, float> CurrentValues;
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetTileCoordinateCurrentlyHovered(FIntVector& TileCoordinate, float& Height);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		float GetAverageHeightAroundIndex(FVector Location);
	UFUNCTION()
		float InternalGetHeight(FVector& Location, FIntVector Position, float& Amount);
	UFUNCTION(BlueprintCallable)
		void AddRatesToValues(float DeltaTime);
	UFUNCTION(BlueprintCallable)
		void ZeroCurrentValues();

	ALevelController();

	UPROPERTY()
		UCurveFloat* HeightWeight;
};
