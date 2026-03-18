// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CC_WidgetComponent.h"

#include "CC_BaseCharacter.h"
#include "AbilitySystem/CC_AbilitySystemComponent.h"
#include "AbilitySystem/CC_AttributeSet.h"


// Called when the game starts
void UCC_WidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitAbilitySystemData();

	if (!CCAttributeSet.IsValid() || !CCAbilitySystemComponent.IsValid())
	{
		CCCharacter->OnASCInitialized.AddDynamic(this, &UCC_WidgetComponent::OnASCInitialized);
	}
	
}

void UCC_WidgetComponent::InitAbilitySystemData()
{
	CCCharacter = Cast<ACC_BaseCharacter>(GetOwner());
	CCAttributeSet = Cast<UCC_AttributeSet>(CCCharacter->GetAttributeSet());
	CCAbilitySystemComponent = Cast<UCC_AbilitySystemComponent>(CCCharacter->GetAbilitySystemComponent());
}

void UCC_WidgetComponent::OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AttributeSet)
{
	CCAttributeSet = Cast<UCC_AttributeSet>(AttributeSet);
	CCAbilitySystemComponent = Cast<UCC_AbilitySystemComponent>(ASC);
}


