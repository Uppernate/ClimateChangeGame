// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "Curves/CurveVector.h"
#include "BuildingLibrary.generated.h"

UENUM(BlueprintType)
enum class ERate : uint8
{
	VE_Water UMETA(DisplayName = "Water"),
	VE_Heat UMETA(DisplayName = "Heat"),
	VE_Pollution UMETA(DisplayName = "Pollution"),
	VE_Energy UMETA(DisplayName = "Energy"),
};

USTRUCT(BlueprintType)
struct FBuildingVariation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UStaticMesh* Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<UMaterialInterface*> Materials;
};

USTRUCT(BlueprintType)
struct FBuildingData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TArray<FBuildingVariation> Variations;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TMap<ERate, float> Rates;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int32 InstanceCount = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FVector4 DistributionRandomness;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCurveFloat* VariantDistribution;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCurveVector* LocationDistribution;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCurveVector* RotationDistribution;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UCurveVector* ScaleDistribution;
};

/**
 * 
 */
UCLASS()
class CLIMATECHANGEGAME_API UBuildingLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION()
		static float DistributionToCurveRange(float Distribution, UCurveBase* Curve);
};
