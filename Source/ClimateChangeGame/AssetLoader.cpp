// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetLoader.h"

/*FAssetList::FAssetList(const TCHAR* Path)
{
	FString* JsonRaw;
	LoadAsset<FString>(Path, JsonRaw);
	TSharedPtr<FJsonObject> JsonParsed;
	const TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(*JsonRaw);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), *JsonParsed->GetStringField(TEXT("Test")));
	}
}*/