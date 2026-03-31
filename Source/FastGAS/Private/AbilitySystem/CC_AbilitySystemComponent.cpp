// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/CC_AbilitySystemComponent.h"

#include "GameplayTags/CCTags.h"

/*
 * const auto Name = EX_GET_ENUM_NAME(ETestType, TestType);
 */
#define EX_GET_ENUM_NAME(StaticEnumDefineName, EnumValue)\
StaticEnum<StaticEnumDefineName>()->GetNameStringByValue(static_cast<uint8>(EnumValue));

void UCC_AbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);

	HandleAutoActivatedAbility(AbilitySpec);

	 UE_LOG(LogTemp, Display, TEXT("OnGiveAbility Completed"));

	// int a = (int) GetNetMode();
	//
	// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow,name );

}

void UCC_AbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	//我也没有懂为什么还要在这里进行激活一次 OnGiveAbility 函数理论上是服务器和客户端都调用一次的

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		HandleAutoActivatedAbility(AbilitySpec);
	}
}

void UCC_AbilitySystemComponent::HandleAutoActivatedAbility(const FGameplayAbilitySpec& AbilitySpec)
{
	if (!IsValid(AbilitySpec.Ability)) return;
	
	for (auto& Tag : AbilitySpec.Ability->GetAssetTags())
	{
		if (Tag.MatchesTagExact(CCTags::CCAbilities::ActivateOnGiven))
		{
			TryActivateAbility(AbilitySpec.Handle);
			return;
		}
	}
}

void UCC_AbilitySystemComponent::SetAbilityLevel(TSubclassOf<UGameplayAbility> Ability, int32 Level)
{
	if (IsValid(GetAvatarActor()) && !GetAvatarActor()->HasAuthority()) return;

	if (FGameplayAbilitySpec* Spec = FindAbilitySpecFromClass(Ability))
	{
		Spec->Level = Level;
		MarkAbilitySpecDirty(*Spec);
	}
}
