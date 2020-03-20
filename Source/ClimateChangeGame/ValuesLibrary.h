// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Materials/MaterialInterface.h"
#include "ValuesLibrary.generated.h"

USTRUCT(BlueprintType)
struct FValueRules
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool CanHaveNegativeRates = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float DefaultRate = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MinimumRate = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaximumRate = -1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float DefaultValue = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MinimumValue = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MaximumValue = -1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UMaterialInterface* Icon;
};

/**
 * 
 */
UCLASS()
class CLIMATECHANGEGAME_API UValuesLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
};
