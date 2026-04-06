// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CC_WidgetComponent.h"

#include "Character/CC_BaseCharacter.h"
#include "AbilitySystem/CC_AbilitySystemComponent.h"
#include "AbilitySystem/CC_AttributeSet.h"
#include "Blueprint/WidgetTree.h"
#include "UI/CC_AttributeWidget.h"


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

	if (CCAttributeSet.IsValid())
	{
		OnAttributeInitialized();
	}
}

void UCC_WidgetComponent::OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AttributeSet)
{
	CCAttributeSet = Cast<UCC_AttributeSet>(AttributeSet);
	CCAbilitySystemComponent = Cast<UCC_AbilitySystemComponent>(ASC);

	if (CCAttributeSet.IsValid())
	{
		OnAttributeInitialized();
	}
}

void UCC_WidgetComponent::OnAttributeInitialized()
{
	for (const TTuple<FGameplayAttribute,FGameplayAttribute>& pair : AttributeMap)
	{
		//查找所有ui 看看是不是 我们的CC AttributeWidget

		BindWidgetToAttributeChanges(GetUserWidgetObject(), pair);
		//ForEachWidget 不包含自身
		GetUserWidgetObject()->WidgetTree->ForEachWidget([this,&pair](UWidget* ChildWidget)
		{
			BindWidgetToAttributeChanges(ChildWidget, pair);
		});
	}
}

void UCC_WidgetComponent::BindWidgetToAttributeChanges(UWidget* WidgetObject, const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const
{
	UCC_AttributeWidget* CCAttributeWidget = Cast<UCC_AttributeWidget>(WidgetObject);
	if (!IsValid(CCAttributeWidget)) return;
	if (!CCAttributeWidget->MatchesAttributes(Pair)) return;

	//首次值传递
	CCAttributeWidget->OnAttributeChange(Pair,CCAttributeSet.Get());

	CCAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Key).AddLambda([this, &Pair, CCAttributeWidget](const FOnAttributeChangeData& Data)
	{
		CCAttributeWidget->OnAttributeChange(Pair,CCAttributeSet.Get());
	});
	CCAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda([this, &Pair, CCAttributeWidget](const FOnAttributeChangeData& Data)
	{
		CCAttributeWidget->OnAttributeChange(Pair,CCAttributeSet.Get());
	});
}


