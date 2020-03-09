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
		bool DefaultToZeroVariant = Data->VariantDistribution ? false : true;
		for (int32 i = 0; i < Data->InstanceCount; i++)
		{
			OnNewInstance(i, Data);
			float DefaultVal = 1.0f / (Data->InstanceCount + 1) * i + 1.0f / (Data->InstanceCount + 1);
			FVector4 Distribution = FVector4(DefaultVal, DefaultVal, DefaultVal, DefaultVal);
			Distribution.X = FMath::Lerp(Distribution.X, FMath::RandRange(0.0f, 1.0f), Data->DistributionRandomness.X);
			Distribution.Y = FMath::Lerp(Distribution.Y, FMath::RandRange(0.0f, 1.0f), Data->DistributionRandomness.Y);
			Distribution.Z = FMath::Lerp(Distribution.Z, FMath::RandRange(0.0f, 1.0f), Data->DistributionRandomness.Z);
			Distribution.W = FMath::Lerp(Distribution.W, FMath::RandRange(0.0f, 1.0f), Data->DistributionRandomness.W);
			
			int32 Variant = 0;
			FTransform InstanceTransform;
			if (!DefaultToZeroVariant)
			{
				float ApproxVariant = Data->VariantDistribution->GetFloatValue(
					UBuildingLibrary::DistributionToCurveRange(Distribution.X, Data->VariantDistribution));
				int32 ChosenVariant = FMath::RoundToInt(ApproxVariant);
				// Clamp so result does not go out of array's bounds
				Variant = FMath::Min(FMath::Max(ChosenVariant, 0), InstancedMeshes.Num());
				
			}
			if (Data->LocationDistribution)
				InstanceTransform.SetLocation(Data->LocationDistribution->GetVectorValue(
					UBuildingLibrary::DistributionToCurveRange(Distribution.Y, Data->LocationDistribution)));
			if (Data->RotationDistribution)
			{
				FVector RotationVector = Data->RotationDistribution->GetVectorValue(
					UBuildingLibrary::DistributionToCurveRange(Distribution.Z, Data->RotationDistribution));
				// TODO: This isn't a proper way to have rotation due to euler rotation being used (gimbal lock, etc.)
				FRotator Rotation{RotationVector.Y * 360.0f, RotationVector.Z * 360.0f, RotationVector.X * 360.0f};
				InstanceTransform.SetRotation(Rotation.Quaternion());
			}
			if (Data->ScaleDistribution)
				InstanceTransform.SetScale3D(Data->ScaleDistribution->GetVectorValue(
					UBuildingLibrary::DistributionToCurveRange(Distribution.W, Data->ScaleDistribution)));
			InstancedMeshes[Variant]->AddInstance(InstanceTransform);
		}
		for (auto& Elem : Data->Rates)
		{
			Rates.Add(Elem.Key, Elem.Value);
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

void ABuilding::OnNewInstance_Implementation(int Index, FBuildingData* Data)
{
	float DefaultVal = 1.0f / (Data->InstanceCount + 1) * Index + 1.0f / (Data->InstanceCount + 1);
	FVector4 Distribution = FVector4(DefaultVal, DefaultVal, DefaultVal, DefaultVal);
	Distribution.X = FMath::Lerp(Distribution.X, FMath::RandRange(0.0f, 1.0f), Data->DistributionRandomness.X);
	Distribution.Y = FMath::Lerp(Distribution.Y, FMath::RandRange(0.0f, 1.0f), Data->DistributionRandomness.Y);
	Distribution.Z = FMath::Lerp(Distribution.Z, FMath::RandRange(0.0f, 1.0f), Data->DistributionRandomness.Z);
	Distribution.W = FMath::Lerp(Distribution.W, FMath::RandRange(0.0f, 1.0f), Data->DistributionRandomness.W);

	int32 Variant = 0;
	FTransform InstanceTransform;
	if (Data->VariantDistribution)
	{
		float ApproxVariant = Data->VariantDistribution->GetFloatValue(
			UBuildingLibrary::DistributionToCurveRange(Distribution.X, Data->VariantDistribution));
		int32 ChosenVariant = FMath::RoundToInt(ApproxVariant);
		// Clamp so result does not go out of array's bounds
		Variant = FMath::Min(FMath::Max(ChosenVariant, 0), InstancedMeshes.Num());

	}
	if (Data->LocationDistribution)
		InstanceTransform.SetLocation(Data->LocationDistribution->GetVectorValue(
			UBuildingLibrary::DistributionToCurveRange(Distribution.Y, Data->LocationDistribution)));
	if (Data->RotationDistribution)
	{
		FVector RotationVector = Data->RotationDistribution->GetVectorValue(
			UBuildingLibrary::DistributionToCurveRange(Distribution.Z, Data->RotationDistribution));
		// TODO: This isn't a proper way to have rotation due to euler rotation being used (gimbal lock, etc.)
		FRotator Rotation{ RotationVector.Y * 360.0f, RotationVector.Z * 360.0f, RotationVector.X * 360.0f };
		InstanceTransform.SetRotation(Rotation.Quaternion());
	}
	if (Data->ScaleDistribution)
		InstanceTransform.SetScale3D(Data->ScaleDistribution->GetVectorValue(
			UBuildingLibrary::DistributionToCurveRange(Distribution.W, Data->ScaleDistribution)));
	InstancedMeshes[Variant]->AddInstance(InstanceTransform);
}

void ABuilding::AddInstanceTo(int InstancedIndex, FTransform Transform)
{
	
}

