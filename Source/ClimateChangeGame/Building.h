// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "TileLibrary.h"
#include "BuildingLibrary.h"
#include "Building.generated.h"

UCLASS()
class CLIMATECHANGEGAME_API ABuilding : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuilding();
	
	UPROPERTY()
		TArray<UInstancedStaticMeshComponent*> InstancedMeshes;
	UPROPERTY()
		UDataTable* BuildingTypes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<ERate, float> Rates;
	UPROPERTY(BlueprintReadOnly)
		FIntVector AttachedPosition;
	UPROPERTY(BlueprintReadOnly)
		float AttachedHeight;
	UPROPERTY(BlueprintReadOnly)
		bool IsAttached;
	UPROPERTY()
		float LastFloodCheck;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		bool AttachToPosition(FIntVector Coordinates);
	UFUNCTION(BlueprintCallable)
		bool Deattach();
	UFUNCTION(BlueprintCallable)
		bool CheckForFlooding();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnFlooded();
		void OnFlooded_Implementation();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void OnNewInstance(int Index, FBuildingData Data);
		void OnNewInstance_Implementation(int Index, FBuildingData Data);
	UFUNCTION(BlueprintCallable)
		FTransform GenerateRandomVariantAndTransform(FBuildingData Data, FVector4 Distribution, int& Variant);
	UFUNCTION(BlueprintCallable)
		void AddInstanceTo(int InstancedIndex, FTransform Transform);
};
