// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GridLibrary.generated.h"

/**
 * 
 */
UCLASS()
class CLIMATECHANGEGAME_API UGridLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		static FVector TileIndexToWorldPosition(FIntVector TileIndex);
	UFUNCTION(BlueprintCallable)
		static FIntVector PositionToNearestIndex(FVector Position);
};
