// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldGenerator.h"
#include "AssetLoader.h"
#include "GridLibrary.h"

// Sets default values
AWorldGenerator::AWorldGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	LoadAsset<UDataTable>(TEXT("DataTable'/Game/DataTables/DT_TileTypes.DT_TileTypes'"), TileTypes);
}

// Called when the game starts or when spawned
void AWorldGenerator::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* GenericController = GetWorld()->GetFirstPlayerController();
	if (GenericController)
	{
		Controller = Cast<ALevelController>(GenericController);
		if (Controller)
		{
			Controller->Generator = this;
			Controller->WorldTiles = GetWorld()->SpawnActor<AWorldTiles>();
			GenerateWorld();
		}
	}
}

// Called every frame
void AWorldGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorldGenerator::GenerateWorld_Implementation()
{
	// Basic Island

	// Island Boundaries

	TArray<float> Boundary;
	Boundary.SetNumZeroed(90);
	float CurrentSize = 1000.0f;
	float SizeIncrement = 0.0f;
	for (int32 i = 0; i < Boundary.Num(); i++)
	{
		SizeIncrement += FMath::RandRange(-50.0f, 50.0f);
		SizeIncrement = FMath::Min(FMath::Max(SizeIncrement, -200.0f), 200.0f);
		CurrentSize += SizeIncrement;
		CurrentSize = FMath::Lerp(CurrentSize, 2000.0f, 0.1f);
		CurrentSize = FMath::Min(FMath::Max(CurrentSize, 300.0f), 1600.0f);
		SizeIncrement = FMath::Lerp(SizeIncrement, 0.0f, 0.1f);
		Boundary[i] = CurrentSize;
	}
	
	for (int32 x = -MaxSize; x <= MaxSize; x++)
	{
		for (int32 y = -MaxSize; y <= MaxSize; y++)
		{
			FTile Tile;
			Tile.Name = FString(TEXT("grass"));
			FIntVector Index(x, y, 0);
			float Height = FMath::RandRange(-40.0f, 40.0f);
			FVector RealLocation = UGridLibrary::TileIndexToWorldPosition(Index);
			FRotator LocationAngle = RealLocation.ToOrientationRotator();
			int32 BoundaryIndex = FMath::Max(0, FMath::Min(FMath::RoundToInt((LocationAngle.Yaw + 180.0f) / 4.0f), Boundary.Num() - 1));
			float BoundaryLimit = Boundary[BoundaryIndex];
			UE_LOG(LogTemp, Display, TEXT("Index: %i, Value: %f"), BoundaryIndex, BoundaryLimit)
			Height = FMath::Lerp(0.0f, -160.0f, FMath::Pow(RealLocation.Size() * FMath::RandRange(0.9f, 1.1f) / BoundaryLimit, 2.0f)) + Height;
			Tile.CurrentHeight = Height;
			Controller->WorldTiles->AddTile(Tile, Index);
		}
	}
	// Rivers
	int32 RiverCount = FMath::RandRange(1, 8);
	for (int32 i = 0; i < RiverCount; i++)
	{
		FIntVector Initial = { 0, 0, 0 };
		Initial.X += FMath::RandRange(-MaxSize/2, MaxSize/2);
		Initial.Y += FMath::RandRange(-MaxSize/2, MaxSize/2);
		FVector Location = UGridLibrary::TileIndexToWorldPosition(Initial);
		FRotator Direction = { 0, FMath::RandRange(0.0f, 360.0f), 0 };
		float DirIncrement = 0.0f;
		int32 RiverLength = FMath::RandRange(60, 400);
		for (int32 x = 0; x < RiverLength; x++)
		{
			DirIncrement += FMath::RandRange(-2.0f, 2.0f);
			Direction.Yaw += DirIncrement;
			Location += Direction.Vector() * 50.0f;
			FTile Tile;
			Tile.Name = FString(TEXT("grass"));
			FIntVector Index = UGridLibrary::PositionToNearestIndex(Location);
			float Height = -1000.0f;
			Tile.CurrentHeight = Height;
			Controller->WorldTiles->AddTile(Tile, Index);
		}
	}
	Controller->WorldTiles->RenderTiles();
}

