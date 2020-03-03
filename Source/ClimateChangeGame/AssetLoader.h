// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <tchar.h>
#include "Components/StaticMeshComponent.h"
#include "ConstructorHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "Components/SplineMeshComponent.h"
#include "JsonObject.h"
#include "JsonReader.h"
#include "JsonSerializer.h"

template <class T>
bool LoadAsset(const TCHAR* FromPath, T*& AssignTo)
{
	ConstructorHelpers::FObjectFinder<T> Load(FromPath);
	if (Load.Succeeded())
	{
		AssignTo = Load.Object;
		return true;
	}
	return false;
}

template <class T>
inline T* CreateAndAttachComponent(UObject* Outer, USceneComponent* AttachTo, FAttachmentTransformRules& Rule)
{
	T* Obj = NewObject<T>(Outer);
	Obj->AttachToComponent(AttachTo, Rule);
	return Obj;
}

template <class T>
inline void RegisterConstructionComponent(T*& Component)
{
	Component->RegisterComponent();
	Component->CreationMethod = EComponentCreationMethod::UserConstructionScript;
	Component->OnComponentCreated();
}

template <class T>
T* CreateComponent(UObject* Parent, const FName& Name)
{
	return Parent->CreateDefaultSubobject<T>(MakeUniqueObjectName(Parent, T::StaticClass(), Name));
}

inline bool LoadMeshAsset(const TCHAR* FromPath, UPoseableMeshComponent*& AssignTo)
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh> Load(FromPath);
	if (Load.Succeeded())
	{
		AssignTo->SetSkeletalMesh(Load.Object);
		return true;
	}
	return false;
}

inline bool LoadMeshAsset(const TCHAR* FromPath, UStaticMeshComponent*& AssignTo)
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> Load(FromPath);
	if (Load.Succeeded())
	{
		AssignTo->SetStaticMesh(Load.Object);
		return true;
	}
	return false;
}

inline bool LoadMeshAsset(const TCHAR* FromPath, USplineMeshComponent*& AssignTo)
{
	ConstructorHelpers::FObjectFinder<UStaticMesh> Load(FromPath);
	if (Load.Succeeded())
	{
		AssignTo->SetStaticMesh(Load.Object);
		return true;
	}
	return false;
}

inline bool LoadMaterialAsset(const TCHAR* FromPath, const int32& ElementIndex, UStaticMeshComponent*& AssignTo)
{
	ConstructorHelpers::FObjectFinder<UMaterialInterface> Load(FromPath);
	if (Load.Succeeded())
	{
		AssignTo->SetMaterial(ElementIndex, Load.Object);
		return true;
	}
	return false;
}

inline bool LoadMaterialAsset(const TCHAR* FromPath, const int32& ElementIndex, UPoseableMeshComponent*& AssignTo)
{
	ConstructorHelpers::FObjectFinder<UMaterialInterface> Load(FromPath);
	if (Load.Succeeded())
	{
		AssignTo->SetMaterial(ElementIndex, Load.Object);
		return true;
	}
	return false;
}

/*class FAssetList
{
public:
	FAssetList(const TCHAR* Path);
	TMap<FString, UObject*> Objects;
};

template<class T>
inline T* GetAssetFromList(FAssetList List, const TCHAR* Name)
{
	return static_cast<T*>(List.Objects.operator[](Name));
}*/
