// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CC_GameplayAbility.h"
#include "CC_Primary.generated.h"

/**
 * 
 */
UCLASS()
class FASTGAS_API UCC_Primary : public UCC_GameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category="CC|Ability")
	TArray<AActor*> HitBoxOverlapTest();

	UFUNCTION(BlueprintCallable,Category="CC|Ability")
	void SendHitReactEventToActors(const TArray<AActor*>& HitActors);
	
private:
	UPROPERTY(EditDefaultsOnly,Category="CC|Ability")
	float HitBoxRadius = 100.f;

	UPROPERTY(EditDefaultsOnly,Category="CC|Ability")
	float HitBoxForwardOffset = 200.f;

	UPROPERTY(EditDefaultsOnly,Category="CC|Ability")
	float HitBoxElevationOffset = 100.f;

	void DrawHitBoxOverlapDebugs(const TArray<FOverlapResult>& OverlapResults,const FVector& HitBoxLocation) const;

	
};
