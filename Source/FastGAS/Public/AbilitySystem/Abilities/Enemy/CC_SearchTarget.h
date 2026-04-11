// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CC_GameplayAbility.h"
#include "CC_SearchTarget.generated.h"

class AAIController;
class ACC_EnemyCharacter;
class ACC_BaseCharacter;
/**
 * 
 */
UCLASS()
class FASTGAS_API UCC_SearchTarget : public UCC_GameplayAbility
{
	GENERATED_BODY()
public:

	TWeakObjectPtr<ACC_BaseCharacter> TargetCharacter;
	TWeakObjectPtr<ACC_EnemyCharacter> OwnerCharacter;
	TWeakObjectPtr<AAIController> OwnerAIController;

	
	UCC_SearchTarget();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

private:
	void StartSearch();

};
