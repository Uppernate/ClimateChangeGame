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
	TArray<FIntVector> TilePositions;
	UFUNCTION()
		int32 AddTile(FIntVector Position, float Height);
	UFUNCTION()
		void RemoveTile(int32 Index);
};
