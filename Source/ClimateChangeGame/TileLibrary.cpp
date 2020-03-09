// Fill out your copyright notice in the Description page of Project Settings.


#include "TileLibrary.h"
#include "WorldTiles.h"

void UTileLibrary::GetNeighboursOfTile(AWorldTiles* Tiles, FIntVector Position, TArray<FIntVector>& Neighbours)
{
	Neighbours.Add(Position + FIntVector(1, 0, 0));
	Neighbours.Add(Position + FIntVector(-1, 0, 0));
	Neighbours.Add(Position + FIntVector(0, 1, 0));
	Neighbours.Add(Position + FIntVector(0, -1, 0));
	Neighbours.Add(Position + FIntVector(1, -1, 0));
	Neighbours.Add(Position + FIntVector(-1, 1, 0));
	for (int32 i = 0; i < Neighbours.Num(); i++)
	{
		FTile* Tile = Tiles->TileMap.Find(Neighbours[i]);
		if (!Tile)
		{
			Neighbours.RemoveAt(i, 1);
			i--;
		}
	}
}
