// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
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

	///这个脚本想要做成通用的属性绑定到widget上
	///这个组件会包含一个 uiwidget 也就是UI,UI里面又包含很多子控件,比如血条, 魔法条
	///所以他弄一个TMap<FGameplayAttribute,FGameplayAttribute> AttributeMap;
	///里面放上一堆想要绑定的 (Attribute,MaxAttribute) 然后遍历所有子UI看看那个子ui想绑定到哪个属性上,就绑定到哪个属性上
	///两层循环搞定,第一层循环这个 TMap AttributeMap,第二层循环所有子UI

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "CC|Widget")
	TMap<FGameplayAttribute,FGameplayAttribute> AttributeMap;
	
private:
	TWeakObjectPtr<ACC_BaseCharacter> CCCharacter;
	TWeakObjectPtr<UCC_AbilitySystemComponent> CCAbilitySystemComponent;
	TWeakObjectPtr<UCC_AttributeSet> CCAttributeSet;

	void InitAbilitySystemData();

	UFUNCTION(BlueprintCallable)
	void OnASCInitialized(UAbilitySystemComponent* ASC,UAttributeSet* AttributeSet);

	void OnAttributeInitialized();

	void BindWidgetToAttributeChanges(UWidget* WidgetObject,const TTuple<FGameplayAttribute,FGameplayAttribute>& Pair) const;
};
