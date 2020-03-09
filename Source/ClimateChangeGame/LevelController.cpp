// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelController.h"
#include "GridLibrary.h"
#include "AssetLoader.h"

ALevelController::ALevelController()
{
	LoadAsset<UCurveFloat>(TEXT("CurveFloat'/Game/Curves/C_HeightFetchingIntensity.C_HeightFetchingIntensity'"), HeightWeight);
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
	float HeightValue = 0.0f;
	AverageHeight += InternalGetHeight(Location, Current, HeightValue);
	AverageHeight += InternalGetHeight(Location, Current + FIntVector(1, 0, 0), HeightValue);
	AverageHeight += InternalGetHeight(Location, Current + FIntVector(-1, 0, 0), HeightValue);
	AverageHeight += InternalGetHeight(Location, Current + FIntVector(0, 1, 0), HeightValue);
	AverageHeight += InternalGetHeight(Location, Current + FIntVector(0, -1, 0), HeightValue);
	AverageHeight += InternalGetHeight(Location, Current + FIntVector(1, -1, 0), HeightValue);
	AverageHeight += InternalGetHeight(Location, Current + FIntVector(-1, 1, 0), HeightValue);
	if (HeightValue != 0.0f)
		return AverageHeight / HeightValue;
	else
		return -400.0f;
}

float ALevelController::InternalGetHeight(FVector& Location, FIntVector Position, float& Amount)
{
	FTile* Tile = WorldTiles->TileMap.Find(Position);
	if (Tile)
	{
		FVector Difference = UGridLibrary::TileIndexToWorldPosition(Position) - Location;
		Difference.Z = 0.0f;
		float Multiplier = HeightWeight->GetFloatValue(Difference.Size());
		Amount += Multiplier;
		return Tile->CurrentHeight * Multiplier;
	}
	return 0.0f;
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
