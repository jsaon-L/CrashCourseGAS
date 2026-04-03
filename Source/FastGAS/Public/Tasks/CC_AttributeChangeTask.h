// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "CC_AttributeChangeTask.generated.h"

struct FOnAttributeChangeData;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, FGameplayAttribute, Attribute, float, NewValue, float, OldValue);

//添加meta=(ExposedAsyncProxy = AsyncTask) 可以在节点返回实例引用 然后在合适时机调用EndTask
UCLASS(Blueprintable,meta = (ExposedAsyncProxy = "AsyncTask"))
class FASTGAS_API UCC_AttributeChangeTask : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	//异步输出引脚 使用 UPROPERTY(BlueprintAssignable) 标记 可以有多个动态多播委托变量,每个变量对应一个输出引脚
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged;

	//蓝图里创建异步蓝图节点的函数,使用 (BlueprintInternalUseOnly = "true") 标记
	UFUNCTION(BlueprintCallable,meta = (BlueprintInternalUseOnly = "true"))
	static UCC_AttributeChangeTask* ListenForAttributeChange(UAbilitySystemComponent* AbilitySystemComponent,FGameplayAttribute Attribute);


	//清理函数,此例子里需要手动在蓝图里调用否则不会清理此实例
	UFUNCTION(BlueprintCallable)
	void EndTask();

	TWeakObjectPtr<UAbilitySystemComponent> ASC;
	FGameplayAttribute AttributeToListenFor;

	void AttributeChanged(const FOnAttributeChangeData& Data);
};
