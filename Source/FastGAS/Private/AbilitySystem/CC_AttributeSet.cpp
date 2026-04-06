// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CC_AttributeSet.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffectExtension.h"
#include "GameplayTags/CCTags.h"
#include "Net/UnrealNetwork.h"


void UCC_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//网络复制
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Mana, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxMana, COND_None, REPNOTIFY_Always);
}

void UCC_AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	//当pawn被杀了,通知回击杀者 可以用来判定经验增长等
	if (Data.EvaluatedData.Attribute == GetHealthAttribute() && GetHealth()<=0.f)
	{
		UE_LOG(LogTemp, Display, TEXT("PostGameplayEffectExecute Data.Target.GetAvatarActor():%s"),*Data.Target.GetAvatarActor()->GetName());

		FGameplayEventData Payload;
		// Payload.Instigator = Data.Target.GetAvatarActor();
		Payload.Target = Data.Target.GetAvatarActor();
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Data.EffectSpec.GetEffectContext().GetInstigator(), CCTags::Events::KillScored,Payload);
	}
}

void UCC_AttributeSet::OnRep_Health(const FGameplayAttributeData OldValue)
{
	//本地预测修改属性
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldValue);
}
void UCC_AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData OldValue)
{
	//本地预测修改属性
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, OldValue);
}

void UCC_AttributeSet::OnRep_Mana(const FGameplayAttributeData OldValue)
{
	//本地预测修改属性
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Mana, OldValue);
}

void UCC_AttributeSet::OnRep_MaxMana(const FGameplayAttributeData OldValue)
{
	//本地预测修改属性
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxMana, OldValue);
}