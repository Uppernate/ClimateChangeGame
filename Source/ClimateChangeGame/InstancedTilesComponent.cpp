// Fill out your copyright notice in the Description page of Project Settings.


#include "InstancedTilesComponent.h"
#include "GridLibrary.h"

int32 UInstancedTilesComponent::AddTile(FIntVector Position, float Height)
{
	FTransform InstanceTransform = FTransform(UGridLibrary::TileIndexToWorldPosition(Position) + FVector(0, 0, Height));
	int32 InstanceId = AddInstance(InstanceTransform);
	TilePositions.Add(Position);
	return InstanceId;
}

void UInstancedTilesComponent::RemoveTile(int32 Index)
{
	RemoveInstance(Index);
	TilePositions.RemoveAt(Index);
}
