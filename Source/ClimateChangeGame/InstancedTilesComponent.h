// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "InstancedTilesComponent.generated.h"

/**
 * 
 */
UCLASS()
class CLIMATECHANGEGAME_API UInstancedTilesComponent : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()
public:
	UPROPERTY()
	// An array which links an instanced mesh to a tile coordinate
	TArray<FIntVector> TilePositions;
	// Add an instanced mesh and link it to a tile coordinate
	UFUNCTION()
		int32 AddTile(FIntVector Position, float Height);
	// Remove an instanced mesh along with its linked tile coordinate
	UFUNCTION()
		void RemoveTile(int32 Index);
};
