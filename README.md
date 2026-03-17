# CrashCourseGAS
UE5 快速GAS课程
课程中文标题：虚幻引擎5 Gameplay Ability System (GAS) 速成课程  
课程英文标题：Unreal Engine 5 Gameplay Ability System (GAS) Crash Course  
课程时长：16小时21分钟  

* FAB自行导入免费资源 ParagonBoris和ParagonMinions,并删除报错蓝图


# GAS笔记:

GA中实例化策略大部分建议选择`每个Actor实例化`这样这个GA里可以存储一些信息,比如连招记录等

> 如何控制能力必须播放完成才能再次触发?
可以在GA中 选择 高级->实例化策略->每个Actor实例化 并且不选中重新触发实例化能力

> 如何控制鼠标或者按键按住一直自动触发技能?可以在按键绑定的时候使用 ETriggerEvent::Triggered 上面又设置了必须等能力结束了才能重新触发,所以按住鼠标就可以一直触发了
>

## 如何在C++中定义原生 Tag?


``` c++
//Tags.h
#pragma once
#include "CoreMinimal.h"
#include "NativeGameplayTags.h"

namespace CCTags
{
	namespace CCAbilities
	{
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(ActivateOnGiven);
		UE_DECLARE_GAMEPLAY_TAG_EXTERN(Primary);
	}
}
```

``` c++
//Tags.cpp
#include "GameplayTags/CCTags.h"

namespace CCTags
{
	namespace CCAbilities
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActivateOnGiven,"CCTags.CCAbilities.ActivateOnGiven","授予自动激活")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Primary,"CCTags.CCAbilities.Primary","标签.主要能力")
	}
}


```

## 联网游戏如何在正确时机初始化ASC?
多人游戏中Player的ASC应该放在PlayerState中,非玩家控制的怪物ASC一般放到Character中

Player 需要在PlayerCharacter类中`PossessedBy`与`OnRep_PlayerState`都进行初始化

Enemy 需要在`EnemyCharacter::BeginPlay`中进行初始化

``` c++
//PlayerCharacter.cpp

void ACC_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	//这个函数在服务器被调用的时候,我们可以确定 PlayerState和Pawn都被正确实例化了,所以这个时机可以
	//这个函数只在服务器调用,当这个pawn被控制了,可以初始化AbilitySystemComponent 拥有者和化身
	if (!IsValid(GetAbilitySystemComponent()) || !HasAuthority()) return;
	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(),this);

}

void ACC_PlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//这个函数将在客户端正确初始化AbilitySystemComponent,因为这个时候客户端才实例化PlayerState和Pawn,
	//拥有这两个变量我们才能初始化AbilitySystemComponent
	if (!IsValid(GetAbilitySystemComponent())) return;
	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(),this);
}



```

## 如何制作初始自动授予技能?

- 1.我们在Character中增加自动授予能力数组
``` c++
// BaseCharacter.h
UPROPERTY(EditDefaultsOnly,Category = "CC|Abilities")
TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;
```
- 2.编写自动授予函数
``` c++
// BaseCharacter.cpp
void ACC_BaseCharacter::GiveStartupAbilities()
{
	if (!IsValid(GetAbilitySystemComponent())) return;
	
	for (const auto& Ability: StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);
		GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
	}
}
```
- 3.Player/Enemy调用自动授予函数
	- Player情况下  在 PlayerCharacter::PossessedBy中执行自动授予能力
	- Enemy情况下 在EnemyCharacter::BeginPlay中执行自动授予能力
``` c++
//Player情况下  在 PlayerCharacter::PossessedBy中执行自动授予能力
void ACC_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	//这个函数在服务器被调用的时候,我们可以确定 PlayerState和Pawn都被正确实例化了,所以这个时机可以
	//这个函数只在服务器调用,当这个pawn被控制了,可以初始化AbilitySystemComponent 拥有者和化身
	if (!IsValid(GetAbilitySystemComponent()) || !HasAuthority()) return;
	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(),this);

	//只在服务器授予能力就行,授予能力行为会自动进行网络同步
	GiveStartupAbilities();
}

// Enemy情况下 在EnemyCharacter::BeginPlay中执行自动授予能力
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

```

## 如何制作初始自动授予技能的同时自动激活能力?
1.定义一个Tag用于识别哪些能力需要授予时自动激活

定义一个Tag 名称为`CCTags.CCAbilities.ActivateOnGiven`(名称随意)

2.编写激活函数
``` c++
//AbilitySystemComponent.cpp
void UCC_AbilitySystemComponent::HandleAutoActivatedAbility(const FGameplayAbilitySpec& AbilitySpec)
{
	if (!IsValid(AbilitySpec.Ability)) return;
	
	for (auto& Tag : AbilitySpec.Ability->GetAssetTags())
	{
		//判断是否包含这个标签,如果包含这个标签就激活这个技能
		if (Tag.MatchesTagExact(CCTags::CCAbilities::ActivateOnGiven))
		{
			TryActivateAbility(AbilitySpec.Handle);
			return;
		}
	}
}

```
3.调用激活函数,需要在`OnGiveAbility`与`OnRep_ActivateAbilities`都调用才能正确在网络中同步
``` c++
//AbilitySystemComponent.cpp
void UCC_AbilitySystemComponent::OnGiveAbility(FGameplayAbilitySpec& AbilitySpec)
{
	Super::OnGiveAbility(AbilitySpec);

	HandleAutoActivatedAbility(AbilitySpec);
}

void UCC_AbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	FScopedAbilityListLock ActiveScopeLock(*this);
	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		HandleAutoActivatedAbility(AbilitySpec);
	}
}
```
4.将tag设置给GA

在蓝图中将Tag设置给AssetTag 变量
## 如何制作普通攻击技能GA 以及左右手攻击不同动画交替显示?

将GA的实例化策略改成 每Actor实例化,这样无论释放多少次技能,实际只存在一个GA实例.
这样的话我们就可以在GA中保存变量或者使用Flip节点来播放不同的动画蒙太奇来播放不同的连招动画

## 如何给一个Actor发送 GameplayEvent事件?

```c++
FGameplayEventData Payload;
Payload.Instigator = GetAvatarActorFromActorInfo();
UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(actor,CCTags::Events::Enemy::HitReact,Payload);
```

# Attribute
## AttributeSet 创建一个血量 样板代码
```c++
//CC_AttributeSet.h
#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "CC_AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class FASTGAS_API UCC_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Health)
	FGameplayAttributeData Health;

	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData OldValue);

	ATTRIBUTE_ACCESSORS(ThisClass,Health);
};
```
```c++
//CC_AttributeSet.cpp
#include "AbilitySystem/CC_AttributeSet.h"
#include "Net/UnrealNetwork.h"

void UCC_AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//网络复制
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always);
}

void UCC_AttributeSet::OnRep_Health(const FGameplayAttributeData OldValue)
{
	//本地预测修改属性
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldValue);
}
```