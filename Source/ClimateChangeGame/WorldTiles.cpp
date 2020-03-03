// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldTiles.h"
#include "AssetLoader.h"
#include "GridLibrary.h"

// Sets default values
AWorldTiles::AWorldTiles()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LoadAsset<UDataTable>(TEXT("DataTable'/Game/DataTables/DT_TileTypes.DT_TileTypes'"), TileTypes);
}

// Called when the game starts or when spawned
void AWorldTiles::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWorldTiles::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AWorldTiles::AddTile(FTile& Tile, FIntVector Position)
{
	TileMap.Add(Position, Tile);
	return true;
}

bool AWorldTiles::RemoveTileFromPosition(FIntVector Position)
{
	TileMap.Remove(Position);
	return true;
}

/*bool AWorldTiles::AddTile(FTile& Tile, FIntVector Position)
{
	// Check if instance component exists
	int32* InstanceExists = TypeToId.Find(Tile.Name);
	UInstancedStaticMeshComponent* MeshInstance = nullptr;
	if (InstanceExists)
	{
		MeshInstance = TileInstances[*InstanceExists];
	}
	else
	{
		// Query datatable for creating a new instance component
		static const FString ContextString(TEXT("Creating Tile of Type"));
		FTileData* TileType = TileTypes->FindRow<FTileData>(FName(*Tile.Name), ContextString, true);
		if (TileType)
		{
			UInstancedStaticMeshComponent* NewMeshInstance = NewObject<UInstancedStaticMeshComponent>(this);
			NewMeshInstance->RegisterComponent();
			NewMeshInstance->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			NewMeshInstance->SetRelativeTransform(FTransform());
			NewMeshInstance->SetStaticMesh(TileType->Mesh);
			TypeToId.Add(Tile.Name, TileInstances.Add(NewMeshInstance));
			MeshInstance = NewMeshInstance;
		}
	}
	if (!MeshInstance) return false;
	RemoveTileFromPosition(Position);
	Tile.InstanceId = MeshInstance->AddInstance(FTransform(UGridLibrary::TileIndexToWorldPosition(Position) + FVector(0, 0, 1) * Tile.CurrentHeight));
	TileMap.Add(Position, Tile);
	return true;
}

bool AWorldTiles::RemoveTileFromPosition(FIntVector Position)
{
	FTile* CurrentTile = TileMap.Find(Position);
	if (CurrentTile)
	{
		int32* InstanceExists = TypeToId.Find(CurrentTile->Name);
		if (InstanceExists)
		{
			UInstancedStaticMeshComponent* Instance = TileInstances[*InstanceExists];
			Instance->RemoveInstance(CurrentTile->InstanceId);
			for (auto& Elem : TileMap)
			{
				if (Elem.Value.Name == CurrentTile->Name && Elem.Value.InstanceId > CurrentTile->InstanceId)
				{
					Elem.Value.InstanceId--;
				}
			}
			return true;
		}
	}
	return false;
}*/

void AWorldTiles::RenderTiles()
{
	// Check if instance component exists
	for (auto& Elem : TileMap)
	{
		FTile Tile = Elem.Value;
		int32* InstanceExists = TypeToId.Find(Tile.Name);
		UInstancedTilesComponent* MeshInstance = nullptr;
		if (InstanceExists)
		{
			MeshInstance = TileInstances[*InstanceExists];
		}
		else
		{
			// Query datatable for creating a new instance component
			static const FString ContextString(TEXT("Creating Tile of Type"));
			FTileData* TileType = TileTypes->FindRow<FTileData>(FName(*Tile.Name), ContextString, true);
			if (TileType)
			{
				UInstancedTilesComponent* NewMeshInstance = NewObject<UInstancedTilesComponent>(this);
				NewMeshInstance->RegisterComponent();
				NewMeshInstance->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
				NewMeshInstance->SetRelativeTransform(FTransform());
				NewMeshInstance->SetStaticMesh(TileType->Mesh);
				//NewMeshInstance->SetMaterial(0, TileType->Material);
				NewMeshInstance->CreateDynamicMaterialInstance(0, TileType->Material);
				TypeToId.Add(Tile.Name, TileInstances.Add(NewMeshInstance));
				MeshInstance = NewMeshInstance;
			}
		}
		if (!MeshInstance) continue;
		//Tile.InstanceId = MeshInstance->AddInstance(FTransform(UGridLibrary::TileIndexToWorldPosition(Elem.Key) + FVector(0, 0, 1) * Tile.CurrentHeight));
		Tile.InstanceId = MeshInstance->AddTile(Elem.Key, Tile.CurrentHeight);
	}
}

bool AWorldTiles::GetInstancedTile(UInstancedTilesComponent* TilesComponent, int32 Index, FString& TileType,
	FTransform& TileTransform)
{
	for (auto& Elem : TypeToId)
	{
		if (TilesComponent == TileInstances[Elem.Value])
		{
			TileType = Elem.Key;
			return TilesComponent->GetInstanceTransform(Index, TileTransform, true);
		}
	}
	return false;
}

