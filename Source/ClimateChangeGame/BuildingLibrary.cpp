// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildingLibrary.h"

float UBuildingLibrary::DistributionToCurveRange(float Distribution, UCurveBase* Curve)
{
	float Min;
	float Max;
	Curve->GetTimeRange(Min, Max);
	return Distribution * (Max - Min) + Min;
}
