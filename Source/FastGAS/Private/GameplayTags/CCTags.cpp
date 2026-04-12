#include "GameplayTags/CCTags.h"

namespace CCTags
{

	namespace SetByCaller
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Projectile,"CCTags.SetByCaller.Projectile","set by caller")
	}
	
	namespace CCAbilities
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(ActivateOnGiven,"CCTags.CCAbilities.ActivateOnGiven","授予自动激活")

		
		
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Primary,"CCTags.CCAbilities.Primary","标签.主要能力")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Secondary,"CCTags.CCAbilities.Secondary","标签.次要能力")
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(Tertiary,"CCTags.CCAbilities.Tertiary","标签.第三能力")


	}



	namespace Events
	{
		UE_DEFINE_GAMEPLAY_TAG_COMMENT(KillScored,"CCTags.Events.KillScored","击杀得分")
		namespace Enemy
		{
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(EndAttack,"CCTags.Events.Enemy.EndAttack","敌人攻击结束")
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(HitReact,"CCTags.Events.Enemy.HitReact","敌人受击反应")
			
			UE_DEFINE_GAMEPLAY_TAG_COMMENT(MeleeTraceHit,"CCTags.Events.Enemy.MeleeTraceHit","近战重叠测试命中")
		}
	}

}

