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

### 如何在C++中定义原生 Tag?


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

### 如何制作自动授予自动激活的能力?

### 如何制作普通攻击技能GA 以及左右手攻击不同动画交替显示?