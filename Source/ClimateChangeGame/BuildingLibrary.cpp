// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingLibrary.h"
#include "Curves/CurveFloat.h"

float UBuildingLibrary::DistributionToCurveRange(float Distribution, UCurveBase* Curve)
{
	float Min;
	float Max;
	Curve->GetTimeRange(Min, Max);
	return Distribution * (Max - Min) + Min;
}

FVector4 UBuildingLibrary::GenerateRandomDistribution(int Index, int MaxIndex, FVector4 DistributionRandomness)
{
	float DefaultVal = 1.0f / (MaxIndex + 1) * Index + 1.0f / (MaxIndex + 1);
	FVector4 Distribution = FVector4(DefaultVal, DefaultVal, DefaultVal, DefaultVal);
	Distribution.X = FMath::Lerp(Distribution.X, FMath::RandRange(0.0f, 1.0f), DistributionRandomness.X);
	Distribution.Y = FMath::Lerp(Distribution.Y, FMath::RandRange(0.0f, 1.0f), DistributionRandomness.Y);
	Distribution.Z = FMath::Lerp(Distribution.Z, FMath::RandRange(0.0f, 1.0f), DistributionRandomness.Z);
	Distribution.W = FMath::Lerp(Distribution.W, FMath::RandRange(0.0f, 1.0f), DistributionRandomness.W);
	return Distribution;
}

float UBuildingLibrary::GetFloatDistribution(UCurveFloat* Curve, float Distribution)
{
	float Result = Curve->GetFloatValue(DistributionToCurveRange(Distribution, Curve));
	return Result;
}

FVector UBuildingLibrary::GetVectorDistribution(UCurveVector* Curve, float Distribution)
{
	FVector Result = Curve->GetVectorValue(DistributionToCurveRange(Distribution, Curve));
	return Result;
}
