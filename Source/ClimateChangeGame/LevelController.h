// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "WorldTiles.h"
#include "LevelController.generated.h"

class AWorldGenerator;

/**
 * 
 */
UCLASS()
class CLIMATECHANGEGAME_API ALevelController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY()
		AWorldTiles* WorldTiles;
	UPROPERTY()
		AWorldGenerator* Generator;
	void BeginPlay() override;
};
