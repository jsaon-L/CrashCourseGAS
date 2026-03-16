// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CC_BlueprintLibrary.generated.h"

UENUM(BlueprintType)
enum class EHitDirection:uint8
{
	Left,
	Right,
	Forward,
	Back
};


UCLASS()
class FASTGAS_API UCC_BlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure,Category="CC|BlueprintLibrary")
	static EHitDirection GetHitDirection(const FVector& TargetForward,const FVector& ToInstigator);

	UFUNCTION(BlueprintPure,Category="CC|BlueprintLibrary")
	static FName GetHitDirectionName(const EHitDirection& HitDirection);
};
