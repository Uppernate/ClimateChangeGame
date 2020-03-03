// Fill out your copyright notice in the Description page of Project Settings.


#include "GridLibrary.h"

FVector UGridLibrary::TileIndexToWorldPosition(FIntVector TileIndex)
{
	FVector Position = FVector(173.0 * (TileIndex.X + (TileIndex.Y % 2) * 0.5), 150.0 * TileIndex.Y, 0);
	return Position;
}

FIntVector UGridLibrary::PositionToNearestIndex(FVector Position)
{
	FIntVector Index = FIntVector(0, 0, 0);
	Index.Y = FMath::RoundToInt(Position.Y / 150.0);
	Index.X = FMath::RoundToInt((Position.X - 173.0 * (Index.Y % 2) * 0.5) / 173.0f);
	return Index;
}
