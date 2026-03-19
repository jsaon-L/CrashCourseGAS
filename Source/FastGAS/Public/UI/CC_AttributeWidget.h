// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Blueprint/UserWidget.h"
#include "CC_AttributeWidget.generated.h"


class UCC_AttributeSet;
/**
 * 
 */
UCLASS()
class FASTGAS_API UCC_AttributeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "CC|Attribute")
	FGameplayAttribute Attribute;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "CC|Attribute")
	FGameplayAttribute MaxAttribute;


	void OnAttributeChange(const TTuple<FGameplayAttribute,FGameplayAttribute>& Pair, UCC_AttributeSet* AttributeSet);
	
	bool MatchesAttributes(const TTuple<FGameplayAttribute,FGameplayAttribute>& Pair) const;


	UFUNCTION(BlueprintImplementableEvent,meta=(DisplayName="On Attribute Changed"))
	void BP_OnAttributeChanged(float NewValue, float NewMaxValue);
	
};
