// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CC_EnemyCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CC_AbilitySystemComponent.h"


// Sets default values
ACC_EnemyCharacter::ACC_EnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//敌人的技能组件在自己身上,并且敌人的复制模式不需要使用混合,使用最小就好,除非是boss或者精英怪
	AbilitySystemComponent = CreateDefaultSubobject<UCC_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

// Called when the game starts or when spawned
void ACC_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//直接初始化
	if (!IsValid(AbilitySystemComponent))return;
	
	AbilitySystemComponent->InitAbilityActorInfo(this,this);

	
	if (!HasAuthority())return;
	//赋予能力只在服务器上执行
	GiveStartupAbilities();
}

UAbilitySystemComponent* ACC_EnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

