// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/DataTable.h"
#include "TileLibrary.h"
#include "InstancedTilesComponent.h"
#include "WorldTiles.generated.h"

UCLASS(BlueprintType)
class CLIMATECHANGEGAME_API AWorldTiles : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldTiles();
	UPROPERTY(BlueprintReadOnly)
		TMap<FString, int32> TypeToId;
	UPROPERTY(BlueprintReadOnly)
		TArray<UInstancedTilesComponent*> TileInstances;
	UPROPERTY(BlueprintReadOnly)
		TMap<FIntVector, FTile> TileMap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		UDataTable* TileTypes;
	UPROPERTY(BlueprintReadOnly)
		float MaxHeight = -400.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
		bool AddTile(FTile& Tile, FIntVector Position);
	UFUNCTION(BlueprintCallable)
		bool RemoveTileFromPosition(FIntVector Position);
	UFUNCTION(BlueprintCallable)
		void RenderTiles();
	UFUNCTION(BlueprintCallable)
		bool GetInstancedTile(UInstancedTilesComponent* TilesComponent, int32 Index, FString& TileType, FTransform& TileTransform);
};
