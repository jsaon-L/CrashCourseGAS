// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/CC_BlueprintLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/CC_AttributeSet.h"
#include "Character/CC_BaseCharacter.h"
#include "GameplayTags/CCTags.h"
#include "Kismet/GameplayStatics.h"

//第一个参数是敌人的前向向量,第二个向量是敌人到攻击者的向量
EHitDirection UCC_BlueprintLibrary::GetHitDirection(const FVector& TargetForward, const FVector& ToInstigator)
{

	const float Dot = FVector::DotProduct(TargetForward, ToInstigator);

	if (Dot < -0.5f)
	{
		return EHitDirection::Back;
	}

	if (Dot < 0.5f)
	{
		const FVector Cross = FVector::CrossProduct(TargetForward, ToInstigator);
		return Cross.Z > 0 ? EHitDirection::Right : EHitDirection::Left;
	}
	return EHitDirection::Forward;
}

FName UCC_BlueprintLibrary::GetHitDirectionName(const EHitDirection& HitDirection)
{
	switch (HitDirection)
	{
	case EHitDirection::Left:
		return FName("Left");
	case EHitDirection::Right:
		return FName("Right");
	case EHitDirection::Forward:
		return FName("Forward");
	case EHitDirection::Back:
		return FName("Back");
	default:
		return FName("None");
	}
}

FClosestActorWithTagResult UCC_BlueprintLibrary::FindClosestActorWithTag(const UObject* WordContextObject,const FVector& Origin, const FName& Tag)
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithTag(WordContextObject,Tag,Actors);

	float ClosestDistance = TNumericLimits<float>::Max();
	AActor* ClosestActor = nullptr;

	for (AActor* Actor : Actors)
	{
		if (!IsValid(Actor)) continue;
		ACC_BaseCharacter* BaseCharacter = Cast<ACC_BaseCharacter>(Actor);
		if (!IsValid(BaseCharacter) || !BaseCharacter->IsAlive()) continue;

		const float Distance = FVector::Dist(Origin, Actor->GetActorLocation());
		if (Distance < ClosestDistance)
		{
			ClosestActor = Actor;
			ClosestDistance = Distance;
		}
	}

	FClosestActorWithTagResult Result;
	Result.Actor = ClosestActor;
	Result.Distance = ClosestDistance;
	
	return Result;
}

void UCC_BlueprintLibrary::SendDamageEventToPlayer(AActor* Target, const TSubclassOf<UGameplayEffect>& DamageEffect,
	const FGameplayEventData& Payload, const FGameplayTag& DataTag, float Damage)
{
	ACC_BaseCharacter* BaseCharacter = Cast<ACC_BaseCharacter>(Target);
	if (!IsValid(BaseCharacter) || !BaseCharacter->IsAlive()) return;

	UCC_AttributeSet* AttributeSet = Cast<UCC_AttributeSet>(BaseCharacter->GetAttributeSet()) ;
	if (!IsValid(AttributeSet)) return;

	const bool bLethal = AttributeSet->GetHealth() - Damage <=0;
	const FGameplayTag EventTag = bLethal?CCTags::Events::Player::Death : CCTags::Events::Player::HitReact;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(BaseCharacter,EventTag,Payload);

	UAbilitySystemComponent* TargetASC = BaseCharacter->GetAbilitySystemComponent();
	if (!IsValid(TargetASC)) return;

	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(DamageEffect,1,EffectContextHandle);
	
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle,DataTag,-Damage);
	TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
	
}
