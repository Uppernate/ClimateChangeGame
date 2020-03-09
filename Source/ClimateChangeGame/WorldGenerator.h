// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "LevelController.h"
#include "WorldGenerator.generated.h"

UCLASS()
class CLIMATECHANGEGAME_API AWorldGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldGenerator();
	UPROPERTY()
		UDataTable* TileTypes;
	UPROPERTY()
		UDataTable* BuildingTypes;
	UPROPERTY()
		ALevelController* Controller;
	UPROPERTY()
		int32 MaxSize = 32;
	UPROPERTY()
		float TreeChance = 0.12f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void GenerateWorld();
		void GenerateWorld_Implementation();
	UFUNCTION()
		void TreeSpread(FIntVector Position, TSubclassOf<ABuilding> BuildingClass);
};
