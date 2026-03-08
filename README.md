# CrashCourseGAS
UE5 快速GAS课程
课程中文标题：虚幻引擎5 Gameplay Ability System (GAS) 速成课程  
课程英文标题：Unreal Engine 5 Gameplay Ability System (GAS) Crash Course  
课程时长：16小时21分钟  

* FAB自行导入免费资源 ParagonBoris和ParagonMinions,并删除报错蓝图


一些记录:

GA中实例化策略大部分建议选择`每个Actor实例化`这样这个GA里可以存储一些信息,比如连招记录等

> 如何控制能力必须播放完成才能再次触发?
可以在GA中 选择 高级->实例化策略->每个Actor实例化 并且不选中重新出发实例化能力

> 如何控制鼠标或者按键按住一直自动触发技能?可以在按键绑定的时候使用 ETriggerEvent::Triggered 上面又设置了必须等能力结束了才能重新触发,所以按住鼠标就可以一直触发了