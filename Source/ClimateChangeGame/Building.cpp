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
		InstancedMeshes.SetNumZeroed(Data->Variations.Num());
		for (int32 i = 0; i < Data->Variations.Num(); i++)
		{
			UInstancedStaticMeshComponent* NewMeshInstance = NewObject<UInstancedStaticMeshComponent>(this);
			NewMeshInstance->RegisterComponent();
			NewMeshInstance->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
			NewMeshInstance->SetRelativeTransform(FTransform());
			NewMeshInstance->SetStaticMesh(Data->Variations[i].Mesh);
			for (int32 m = 0; m < Data->Variations[i].Materials.Num(); m++)
			{
				NewMeshInstance->CreateAndSetMaterialInstanceDynamicFromMaterial(m, Data->Variations[i].Materials[m]);
			}
			InstancedMeshes[i] = NewMeshInstance;
		}
		for (int32 i = 0; i < Data->InstanceCount; i++)
		{
			OnNewInstance(i, *Data);
		}
		for (auto& Elem : Data->Rates)
		{
			Rates.Add(Elem.Key, Elem.Value);
		}
		for (auto& Elem : Data->PlaceRequirement)
		{
			Required.Add(Elem.Key, Elem.Value);
		}
		for (auto& Elem : Data->DestroyGainAmount)
		{
			GainOnDestroy.Add(Elem.Key, Elem.Value);
		}
	}
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (IsAttached && CurrentTime > LastFloodCheck)
	{
		CheckForFlooding();
		LastFloodCheck = CurrentTime + 1.0f;
	}
}

bool ABuilding::AttachToPosition(FIntVector Coordinates)
{
	if (!IsAttached)
	{
		ALevelController* Controller = GetWorld()->GetFirstPlayerController<ALevelController>();
		if (Controller)
		{
			FTile* Tile = Controller->WorldTiles->TileMap.Find(Coordinates);
			if (Tile && !Tile->Building)
			{
				Tile->Building = this;
				SetActorLocation(UGridLibrary::TileIndexToWorldPosition(Coordinates) + FVector(0, 0, Tile->CurrentHeight));
				AttachedPosition = Coordinates;
				AttachedHeight = Tile->CurrentHeight;
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

bool ABuilding::CheckForFlooding()
{
	ALevelController* Controller = GetWorld()->GetFirstPlayerController<ALevelController>();
	if (Controller)
	{
		float* WaterLevel = Controller->CurrentValues.Find(ERate::VE_Water);
		if (WaterLevel && *WaterLevel > AttachedHeight)
		{
			OnFlooded();
			return true;
		}
	}
	return false;
}

void ABuilding::OnFlooded_Implementation()
{
	Deattach();
	Destroy();
}

void ABuilding::OnNewInstance_Implementation(int Index, FBuildingData Data)
{
	FVector4 Distribution = UBuildingLibrary::GenerateRandomDistribution(Index, Data.InstanceCount, FVector4(0));

	int32 Variant = 0;
	FTransform InstanceTransform = GenerateRandomVariantAndTransform(Data, Distribution, Variant);
	AddInstanceTo(Variant, InstanceTransform);
}

FTransform ABuilding::GenerateRandomVariantAndTransform(FBuildingData Data, FVector4 Distribution, int& Variant)
{
	Variant = 0;
	FTransform InstanceTransform;
	if (Data.VariantDistribution)
	{
		float ApproxVariant = UBuildingLibrary::GetFloatDistribution(Data.VariantDistribution, Distribution.X);
		int32 ChosenVariant = FMath::RoundToInt(ApproxVariant);
		// Clamp so result does not go out of array's bounds
		Variant = FMath::Min(FMath::Max(ChosenVariant, 0), InstancedMeshes.Num());

	}
	if (Data.LocationDistribution)
		InstanceTransform.SetLocation(UBuildingLibrary::GetVectorDistribution(Data.LocationDistribution, Distribution.Y));
	if (Data.RotationDistribution)
	{
		FVector RotationVector = UBuildingLibrary::GetVectorDistribution(Data.RotationDistribution, Distribution.Z);
		// TODO: This isn't a proper way to have rotation due to euler rotation being used (gimbal lock, etc.)
		FRotator Rotation{ RotationVector.Y * 360.0f, RotationVector.Z * 360.0f, RotationVector.X * 360.0f };
		InstanceTransform.SetRotation(Rotation.Quaternion());
	}
	if (Data.ScaleDistribution)
		InstanceTransform.SetScale3D(UBuildingLibrary::GetVectorDistribution(Data.ScaleDistribution, Distribution.W));
	return InstanceTransform;
}

void ABuilding::AddInstanceTo(int InstancedIndex, FTransform Transform)
{
	InstancedMeshes[InstancedIndex]->AddInstance(Transform);
}

void ABuilding::PayRequirements()
{
	ALevelController* Controller = GetWorld()->GetFirstPlayerController<ALevelController>();
	if (Controller)
	{
		for (auto& Elem : Required)
		{
			float* CurrentRate = Controller->CurrentValues.Find(Elem.Key);
			if (CurrentRate)
				Controller->CurrentValues.Add(Elem.Key, *CurrentRate - Elem.Value);
			else
				Controller->CurrentValues.Add(Elem.Key, 0.0f - Elem.Value);
		}
	}
}

void ABuilding::CollectDestroyGainAmount()
{
	ALevelController* Controller = GetWorld()->GetFirstPlayerController<ALevelController>();
	if (Controller)
	{
		for (auto& Elem : GainOnDestroy)
		{
			float* CurrentRate = Controller->CurrentValues.Find(Elem.Key);
			if (CurrentRate)
				Controller->CurrentValues.Add(Elem.Key, *CurrentRate + Elem.Value);
			else
				Controller->CurrentValues.Add(Elem.Key, 0.0f + Elem.Value);
		}
	}
}

