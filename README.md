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


## GAS 调试命令
```
ShowDebug AbilitySystem
```
PageUp/PageDown可以切换调试对象


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

> Player 需要在PlayerCharacter类中`PossessedBy`与`OnRep_PlayerState`都进行初始化.
因为Player的ASC组件放在PlayerState中,所以Player的ASC初始化要考虑服务器和客户端都正确初始化.
- 服务端 PossessedBy函数只在服务器调用,并且此时PlayerState类已经初始化了,所以可以在PossessedBy中对ASC进行初始化
- 客户端 OnRep_PlayerState 客户端初始化ASC需要先得到ASC,因为ASC在PlayerState中,所以我们可以等待PlayerState同步到客户端再进行ASC的初始化


> Enemy 需要在`EnemyCharacter::BeginPlay`中进行初始化.
因为BeginPlay是在客户端/服务器端都执行,并且ASC组件就在Character上,所以这一个函数就可以正确在客户端/服务器初始化ASC

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

## c++如何使用增强输入绑定按键?


## 如何使用GE初始化AttributeSet?
目前看来这个办法并不好用,适合单机游戏,不太灵活

```c++
//BaseCharacter.h

	UPROPERTY(EditDefaultsOnly,Category = "CC|Effects")
	TSubclassOf<UGameplayEffect> InitializeAttributeEffect;

//BaseCharacter.cpp
void ACC_BaseCharacter::InitializeAttributes() const
{
	if (!IsValid(InitializeAttributeEffect)) return;
	
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(InitializeAttributeEffect,1,ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}


//记得在Character蓝图中给变量InitializeAttributeEffect赋值


//激活
//如果是Player(ASC 放在PlayerState中) 可以只在PossessedBy函数中调用InitializeAttributes,因为Effect是同步的,所以只在服务器调用即可
//如果是Enemy(ASC 放在Character中) 可以在BeginPlay函数中调用InitializeAttributes

```

# 如何监听一个Attribute变化

```C++
//通过asc 就可得到多播委托,然后就可以.AddLambda 或者bind了
CCAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(FGameplayAttribute)

```

## AttributeSet是怎么和AbilitySystemComponent关联起来的?

当两个变量在同一个actor上可以通过反射自动关联,注意必须有UPROPERTY()来使这个AttributeSet可以被反射发现

	UPROPERTY(VisibleAnywhere,Category = "CC|Ability")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;


## 如何将Attribute的数值与UIWidget绑定起来显示血量等数值?

我们需要在ASC正确初始化后进行属性绑定
[联网游戏如何在正确时机初始化ASC](#联网游戏如何在正确时机初始化ASC)

* 教程里思路:
	* 1.给Character增加一个组件,UWidgetComponent(用来在角色头顶显示UI的)
	* 2.创建UIWidget 显示血量什么的
	* 3.在UWidgetComponent中绑定 Attribute 变化委托


* 如何知道ASC已经初始化了
增加OnASCInitialized 动态多播委托,用来通知ASC已经初始化完成了,我们绑定需要在ASC初始化以后进行
```c++
//Player的情况
//PlayerCharacter.cpp
//此函数只在服务器调用
void ACC_PlayerCharacter::PossessedBy(AController* NewController)
{
	if (!IsValid(GetAbilitySystemComponent()) || !HasAuthority()) return;
	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(),this);
	OnASCInitialized.Broadcast(GetAbilitySystemComponent(),GetAttributeSet());

}

void ACC_PlayerCharacter::OnRep_PlayerState()
{
	if (!IsValid(GetAbilitySystemComponent())) return;
	GetAbilitySystemComponent()->InitAbilityActorInfo(GetPlayerState(),this);
	OnASCInitialized.Broadcast(GetAbilitySystemComponent(),GetAttributeSet());
}

//Enemy的情况
//EnemyCharacter.cpp
void ACC_EnemyCharacter::BeginPlay()
{
	if (!IsValid(AbilitySystemComponent))return;
	AbilitySystemComponent->InitAbilityActorInfo(this,this);
	OnASCInitialized.Broadcast(GetAbilitySystemComponent(),GetAttributeSet());
}

```
* 正确等待ASC初始化并绑定 (我们在WidgetComponent组件中进行绑定(此组件是放在Character中的))

```c++
//CC_WidgetComponent.cpp
void UCC_WidgetComponent::BeginPlay()
{
	Super::BeginPlay();
	//我们先拿一次看看能不能拿到ASC组件,如果能拿到说明ASC已经初始化了,我们可以进行绑定了
	InitAbilitySystemData();

	//如果没拿到ASC组件,说明可能是在客户端,我们需要等ASC初始化才能开始绑定
	//所以在这监听ASC初始化
	if (!CCAttributeSet.IsValid() || !CCAbilitySystemComponent.IsValid())
	{
		CCCharacter->OnASCInitialized.AddDynamic(this, &UCC_WidgetComponent::OnASCInitialized);
	}
}

void UCC_WidgetComponent::InitAbilitySystemData()
{
	CCCharacter = Cast<ACC_BaseCharacter>(GetOwner());
	CCAttributeSet = Cast<UCC_AttributeSet>(CCCharacter->GetAttributeSet());
	CCAbilitySystemComponent = Cast<UCC_AbilitySystemComponent>(CCCharacter->GetAbilitySystemComponent());
	//BeginPlay直接初始化完成,那就开始绑定
	if (CCAttributeSet.IsValid())
	{
		OnAttributeInitialized();
	}
}

void UCC_WidgetComponent::OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AttributeSet)
{
	CCAttributeSet = Cast<UCC_AttributeSet>(AttributeSet);
	CCAbilitySystemComponent = Cast<UCC_AbilitySystemComponent>(ASC);
	//BeginPlay没初始化成功,那就等待初始化成功事件,然后开始绑定
	if (CCAttributeSet.IsValid())
	{
		OnAttributeInitialized();
	}
}
//绑定函数


void UCC_WidgetComponent::OnAttributeInitialized()
{
	//绑定 attribute
	//我们可以把ASC发送给UI组件,让UI组件自己绑定,或者在此拿到我们自定义的UIWidget进行绑定
	
	//GetUserWidgetObject()函数可以拿到当前组件上显示的UIWidget
	//注意 WidgetTree->ForEachWidget 并不包含自身

	GetUserWidgetObject()->WidgetTree->ForEachWidget([this,&pair](UWidget* ChildWidget)
	{
		//我们可以在此做类型转换,判断此UIWidget是不是我们用来显示Attribute的
		if(是我们用来显示Attribute的UI)
		{
			CCAbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(FGameplayAttribute)
			.AddLambda
			或者
			Bind
			.AddLambda([this, &Pair, CCAttributeWidget](const FOnAttributeChangeData& Data)
			{
				Data.NewValue就是新值
			});
		}
	});
	
}
```

## 如何给技能升级?

```c++
//CC_AbilitySystemComponent.cpp
//修改等级然后设置脏,就更新了
void UCC_AbilitySystemComponent::SetAbilityLevel(TSubclassOf<UGameplayAbility> Ability, int32 Level)
{
	//只能在服务器更改等级
	if (IsValid(GetAvatarActor()) && !GetAvatarActor()->HasAuthority()) return;

	if (FGameplayAbilitySpec* Spec = FindAbilitySpecFromClass(Ability))
	{
		Spec->Level = Level;
		MarkAbilitySpecDirty(*Spec);
	}
}
```
注意如果你想调用`SetAbilityLevel`函数记得在服务器调用,可以使用 Run on Server RPC 来实现
然后可以在 GA中使用`GetAbilityLevel`获取技能等级,后续可以使用此技能等级传递给GE/根据技能等级实现各种效果之类的
也可以删除技能,重新赋予技能,在赋予技能的时候传入等级参数
