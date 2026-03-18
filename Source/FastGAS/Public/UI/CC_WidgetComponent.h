// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CC_WidgetComponent.generated.h"


class UAbilitySystemComponent;
class UCC_AttributeSet;
class UCC_AbilitySystemComponent;
class ACC_BaseCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class FASTGAS_API UCC_WidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()


protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
	TWeakObjectPtr<ACC_BaseCharacter> CCCharacter;
	TWeakObjectPtr<UCC_AbilitySystemComponent> CCAbilitySystemComponent;
	TWeakObjectPtr<UCC_AttributeSet> CCAttributeSet;

	void InitAbilitySystemData();

	UFUNCTION(BlueprintCallable)
	void OnASCInitialized(UAbilitySystemComponent* ASC,UAttributeSet* AttributeSet);
};
