// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelController.h"
#include "GridLibrary.h"

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
