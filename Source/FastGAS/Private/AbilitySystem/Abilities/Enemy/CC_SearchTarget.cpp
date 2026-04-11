// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Enemy/CC_SearchTarget.h"

#include "AIController.h"
#include "Character/CC_EnemyCharacter.h"

UCC_SearchTarget::UCC_SearchTarget()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UCC_SearchTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	OwnerCharacter = Cast<ACC_EnemyCharacter>(GetAvatarActorFromActorInfo());
	if (OwnerCharacter.IsValid())
	{
		OwnerAIController = Cast<AAIController>(OwnerCharacter.Get()->GetController());
	}

	StartSearch();

	
}

void UCC_SearchTarget::StartSearch()
{

	
}
