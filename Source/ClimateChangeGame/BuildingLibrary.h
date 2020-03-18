// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "Curves/CurveVector.h"
#include "BuildingLibrary.generated.h"

class ABuilding;

UENUM(BlueprintType)
enum class ERate : uint8
{
	VE_Water UMETA(DisplayName = "Water"),
	VE_Heat UMETA(DisplayName = "Heat"),
	VE_Pollution UMETA(DisplayName = "Pollution"),
	VE_Energy UMETA(DisplayName = "Energy"),
	VE_Wood UMETA(DisplayName = "Wood"),
	VE_Stone UMETA(DisplayName = "Stone"),
	VE_Iron UMETA(DisplayName = "Iron"),
	VE_Last UMETA(Hidden)
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
		UMaterialInterface* Icon;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
		TSubclassOf<ABuilding> BuildingClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
		TArray<FBuildingVariation> Variations;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
		TMap<ERate, float> Rates;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
		TMap<ERate, float> PlaceRequirement;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
		TMap<ERate, float> DestroyGainAmount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
		int32 InstanceCount = 1;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
		FVector4 DistributionRandomness;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
		UCurveFloat* VariantDistribution;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
		UCurveVector* LocationDistribution;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
		UCurveVector* RotationDistribution;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visuals")
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
	UFUNCTION(BlueprintCallable, BlueprintPure)
		static FVector4 GenerateRandomDistribution(int Index, int MaxIndex, FVector4 DistributionRandomness);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		static float GetFloatDistribution(UCurveFloat* Curve, float Distribution);
	UFUNCTION(BlueprintCallable, BlueprintPure)
		static FVector GetVectorDistribution(UCurveVector* Curve, float Distribution);
};
