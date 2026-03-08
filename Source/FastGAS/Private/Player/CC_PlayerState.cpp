// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CC_PlayerState.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/CC_AbilitySystemComponent.h"

ACC_PlayerState::ACC_PlayerState()
{
	//设置这个Actor 的网络更新频率,注意设置成100并不代表每秒同步100次,也受带宽等影响
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UCC_AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	//来自Gemini
	//对于 拥有该角色的客户端 (Owner)：采用 Full 模式，同步所有 GE 详情（以便 UI 显示 Buff 图标、剩余时间等）。
	//对于 其他观察者 (Simulated Proxies)：采用 Minimal 模式，只同步 Tags 和 Attributes。
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* ACC_PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
