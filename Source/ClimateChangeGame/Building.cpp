// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"
#include "Engine/World.h"
#include "LevelController.h"
#include "GridLibrary.h"
#include "AssetLoader.h"

// Sets default values
ABuilding::ABuilding()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	MeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);
	LoadAsset<UDataTable>(TEXT("DataTable'/Game/DataTables/DT_BuildingTypes.DT_BuildingTypes'"), BuildingTypes);
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
	static const FString ContextString(TEXT("Building Begin Play"));
	FBuildingData* Data =  BuildingTypes->FindRow<FBuildingData>(FName(*Name), ContextString, true);
	if (Data)
	{
		MeshComponent->SetStaticMesh(Data->Mesh);
		for (int32 i = 0; i < Data->Materials.Num(); i++)
		{
			MeshComponent->CreateAndSetMaterialInstanceDynamicFromMaterial(i, Data->Materials[i]);
		}
	}
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ABuilding::AttachToPosition(FIntVector Coordinates)
{
	if (!IsAttached)
	{
		ALevelController* Controller = GetWorld()->GetFirstPlayerController<ALevelController>();
		if (Controller)
		{
			FTile* Tile = Controller->WorldTiles->TileMap.Find(Coordinates);
			if (Tile)
			{
				Tile->Building = this;
				SetActorLocation(UGridLibrary::TileIndexToWorldPosition(Coordinates) + FVector(0, 0, Tile->CurrentHeight));
				AttachedPosition = Coordinates;
				IsAttached = true;
				for (auto& Elem : Rates)
				{
					float* CurrentRate = Controller->Rates.Find(Elem.Key);
					if (CurrentRate)
						Controller->Rates.Add(Elem.Key, *CurrentRate + Elem.Value);
					else
						Controller->Rates.Add(Elem.Key, Elem.Value);
				}
				return true;
			}
		}
	}
	return false;
}

bool ABuilding::Deattach()
{
	if (IsAttached)
	{
		ALevelController* Controller = GetWorld()->GetFirstPlayerController<ALevelController>();
		if (Controller)
		{
			FTile* Tile = Controller->WorldTiles->TileMap.Find(AttachedPosition);
			if (Tile)
			{
				Tile->Building = nullptr;
			}
			AttachedPosition = FIntVector(0);
			IsAttached = false;
			for (auto& Elem : Rates)
			{
				float* CurrentRate = Controller->Rates.Find(Elem.Key);
				if (CurrentRate)
					Controller->Rates.Add(Elem.Key, *CurrentRate - Elem.Value);
				else
					Controller->Rates.Add(Elem.Key, 0.0f - Elem.Value);
			}
			return true;
		}
	}
	return false;
}

