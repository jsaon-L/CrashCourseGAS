// Fill out your copyright notice in the Description page of Project Settings.


#include "FastGAS/Public/Character/CC_BaseCharacter.h"

#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "AbilitySystem/CC_AttributeSet.h"
#include "Net/UnrealNetwork.h"

namespace CCTags
{
	const FName Player = FName("Player");
}


void ACC_BaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, bAlive);
}

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

UAttributeSet* ACC_BaseCharacter::GetAttributeSet() const
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

void ACC_BaseCharacter::InitializeAttributes() const
{
	if (!IsValid(InitializeAttributeEffect)) return;
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(InitializeAttributeEffect,1,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}



void ACC_BaseCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (Data.NewValue <= 0)
	{
		HandleDeath();
	}
}

void ACC_BaseCharacter::HandleDeath()
{
	bAlive = false;
	
}

void ACC_BaseCharacter::BindHealthChangedDelegate()
{
	//监听属性变化
	UCC_AttributeSet* Ucc_Attributes = Cast<UCC_AttributeSet>(GetAttributeSet());
	if (!IsValid(Ucc_Attributes)) return;
	
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Ucc_Attributes->GetHealthAttribute()).AddUObject(this,&ThisClass::OnHealthChanged);
}

void ACC_BaseCharacter::HandleRespawn()
{
	bAlive = true;
}

void ACC_BaseCharacter::ResetAttributes()
{
	if (!IsValid(ResetAttributeEffect)) return;
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(ResetAttributeEffect,1,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

