// Fill out your copyright notice in the Description page of Project Settings.


#include "FastGAS/Public/CC_BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"


// Sets default values
ACC_BaseCharacter::ACC_BaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

UAbilitySystemComponent* ACC_BaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}

// Called when the game starts or when spawned
void ACC_BaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACC_BaseCharacter::GiveStartupAbilities()
{
	if (!IsValid(GetAbilitySystemComponent())) return;
	
	for (const auto& Ability: StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
		GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
	}
}





