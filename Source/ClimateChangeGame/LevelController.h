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
	UPROPERTY()
		AWorldTiles* WorldTiles;
	// A class that modified the tilemap
	UPROPERTY()
		AWorldGenerator* Generator;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<ERate, float> Rates;
	void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
		bool GetTileCoordinateCurrentlyHovered(FIntVector& TileCoordinate);
};
