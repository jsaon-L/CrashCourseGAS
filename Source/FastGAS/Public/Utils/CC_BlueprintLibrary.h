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

USTRUCT(BlueprintType)
struct FClosestActorWithTagResult
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	TWeakObjectPtr<AActor> Actor;

	UPROPERTY(BlueprintReadWrite)
	float Distance{0.f};
	
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


	UFUNCTION(BlueprintCallable)
	static FClosestActorWithTagResult FindClosestActorWithTag(const UObject* WordContextObject, const FVector& Origin, const FName& Tag);
	
};
