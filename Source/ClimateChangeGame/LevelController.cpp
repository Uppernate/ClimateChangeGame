// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelController.h"
#include "GridLibrary.h"

bool ALevelController::GetTileCoordinateCurrentlyHovered(FIntVector& TileCoordinate)
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
			return Success;
		}
	}
	return false;
}
