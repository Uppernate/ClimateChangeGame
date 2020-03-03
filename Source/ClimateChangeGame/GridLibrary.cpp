// Fill out your copyright notice in the Description page of Project Settings.


#include "GridLibrary.h"

FVector UGridLibrary::TileIndexToWorldPosition(FIntVector TileIndex)
{
	
	//FVector Position = FVector(173.0 * (TileIndex.X - FMath::Abs(TileIndex.Y) % 2 * 0.5), 150.0 * TileIndex.Y, 0);
	FVector Position = FVector(173.0f, 0.0f, 0.0f) * TileIndex.X;
	Position += FVector(173.0f * 0.5, 150.0f, 0.0f) * TileIndex.Y;
	return Position;
}

FIntVector UGridLibrary::PositionToNearestIndex(FVector Position)
{
	FIntVector Index = FIntVector(0, 0, 0);
	Index.Y = FMath::RoundToInt(Position.Y / 150.0f);
	Position.X -= 173.0f * 0.5f * Index.Y;
	Index.X = FMath::RoundToInt(Position.X / 173.0f);
	return Index;
}
