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
		UInstancedStaticMeshComponent* MeshComponent;
	UPROPERTY()
		UDataTable* BuildingTypes;

	UPROPERTY()
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TMap<ERate, float> Rates;
	UPROPERTY(BlueprintReadOnly)
		FIntVector AttachedPosition;
	UPROPERTY(BlueprintReadOnly)
		bool IsAttached;

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
};
