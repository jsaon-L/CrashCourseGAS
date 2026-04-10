// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/CC_EnemyCharacter.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CC_AbilitySystemComponent.h"
#include "AbilitySystem/CC_AttributeSet.h"
#include "Runtime/AIModule/Classes/AIController.h"


// Sets default values
ACC_EnemyCharacter::ACC_EnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	//敌人的技能组件在自己身上,并且敌人的复制模式不需要使用混合,使用最小就好,除非是boss或者精英怪
	AbilitySystemComponent = CreateDefaultSubobject<UCC_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UCC_AttributeSet>("AttributeSet");
}

UAttributeSet* ACC_EnemyCharacter::GetAttributeSet() const
{
	return  AttributeSet;
}

//敌人ASC放在自己身上,并且BeginPlay在客户端服务器都会调用,所以在此初始化ASC
//可以完全正确初始化客户端以及服务器端敌人的ASC
void ACC_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//直接初始化
	if (!IsValid(AbilitySystemComponent))return;
	
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	OnASCInitialized.Broadcast(GetAbilitySystemComponent(),GetAttributeSet());
	
	if (!HasAuthority())return;
	//赋予能力只在服务器上执行
	GiveStartupAbilities();
	InitializeAttributes();

	//监听属性变化
	BindHealthChangedDelegate();
}

void ACC_EnemyCharacter::HandleDeath()
{
	Super::HandleDeath();

	AAIController* aiController = GetController<AAIController>();

	if (aiController)
	{
		aiController->StopMovement();
	}
	
}

UAbilitySystemComponent* ACC_EnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

