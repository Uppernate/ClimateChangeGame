// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelController.h"
#include "GridLibrary.h"
#include "AssetLoader.h"

ALevelController::ALevelController()
{
	//LoadAsset<UCurveFloat>(TEXT("CurveFloat'/Game/Curves/C_HeightFetchingIntensity.C_HeightFetchingIntensity'"), HeightWeight);
}

bool ALevelController::GetTileCoordinateCurrentlyHovered(FIntVector& TileCoordinate, float& Height)
{
	FHitResult Res;
	GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery3, true, Res);
	if (IsValid(Res.GetActor()))
	{
		UInstancedTilesComponent* Tiles = Cast<UInstancedTilesComponent>(Res.Component);
		if (Tiles)
		{
			FTransform TileHit;
			bool Success = Tiles->GetInstanceTransform(Res.Item, TileHit, true);
			TileCoordinate = UGridLibrary::PositionToNearestIndex(TileHit.GetLocation());
			Height = TileHit.GetLocation().Z;
			return Success;
		}
	}
	return false;
}

float ALevelController::GetAverageHeightAroundIndex(FVector Location)
{
	FIntVector Current = UGridLibrary::PositionToNearestIndex(Location);
	float AverageHeight = 0.0f;
	float HeightValueReal = 0.0f;
	float TempHeighValue = 0.0f;
	TArray<FIntVector> AdjacentPositions;
	AdjacentPositions.SetNumZeroed(6);
	AdjacentPositions[0] = Current + FIntVector(1, 0, 0);
	AdjacentPositions[1] = Current + FIntVector(-1, 0, 0);
	AdjacentPositions[2] = Current + FIntVector(0, 1, 0);
	AdjacentPositions[3] = Current + FIntVector(0, -1, 0);
	AdjacentPositions[4] = Current + FIntVector(1, -1, 0);
	AdjacentPositions[5] = Current + FIntVector(-1, 10, 0);
	for (int32 i = 0; i < 6; i ++)
	{
		FTile* Tile = WorldTiles->TileMap.Find(AdjacentPositions[i]);
		if (Tile)
		{
			FVector Difference = UGridLibrary::TileIndexToWorldPosition(AdjacentPositions[i]) - Location;
			Difference.Z = 0.0f;
			float Multiplier = HeightWeight->GetFloatValue(Difference.Size());
			HeightValueReal += Multiplier;
			AverageHeight += Tile->CurrentHeight * Multiplier;
		}
	}
	if (HeightValueReal != 0.0f)
		return AverageHeight / HeightValueReal;
	return -400.0f;
}

void ALevelController::AddRatesToValues(float DeltaTime)
{
	for (auto& Elem : Rates)
	{
		float* Current = CurrentValues.Find(Elem.Key);
		CurrentValues.Add(Elem.Key, Current ? *Current + Elem.Value * DeltaTime : Elem.Value * DeltaTime);
	}
}

void ALevelController::ZeroCurrentValues()
{
	const int32 Max = static_cast<int32>(ERate::VE_Last);
	for (int32 i = 0; i < Max; i++)
	{
		CurrentValues.Add(static_cast<ERate>(i), 0.0f);
	}
}
