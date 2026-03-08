// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/CC_GameplayAbility.h"

void UCC_GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	UE_LOG(LogTemp, Display, TEXT("Active Ability"));
}
