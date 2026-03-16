// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/CC_Primary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Engine/OverlapResult.h"
#include "GameplayTags/CCTags.h"

TArray<AActor*> UCC_Primary::HitBoxOverlapTest()
{

	//忽略自身 Character
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(ActorsToIgnore);


	//忽略除pawn之外的所有通道
	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn,ECR_Block);

	TArray<FOverlapResult> OverlapResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(HitBoxRadius);

	FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector() * HitBoxForwardOffset;
	FVector AvatarLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	FVector OverlapLocation = AvatarLocation + Forward + FVector(0.f,0.f,HitBoxElevationOffset);
	
	GetWorld()->OverlapMultiByChannel(OverlapResults,OverlapLocation,FQuat::Identity,ECC_Visibility,Sphere,QueryParams,ResponseParams);

	TArray<AActor*> HitActors;
	for (const FOverlapResult& Result : OverlapResults)
	{
		if (IsValid(Result.GetActor()))
		{
			HitActors.AddUnique(Result.GetActor());
		}
	}
	
	if (bDrawDebugs)
	{
		DrawHitBoxOverlapDebugs(OverlapResults,OverlapLocation);
	}
	return HitActors;
}
 
void UCC_Primary::SendHitReactEventToActors(const TArray<AActor*>& HitActors)
{
	//发送Gameplay事件
	for (auto actor : HitActors)
	{
		if (IsValid(actor))
		{
			FGameplayEventData Payload;
			Payload.Instigator = GetAvatarActorFromActorInfo();
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(actor,CCTags::Events::Enemy::HitReact,Payload);
		}
	}
}

void UCC_Primary::DrawHitBoxOverlapDebugs(const TArray<FOverlapResult>& OverlapResults,const FVector& HitBoxLocation) const
{
	DrawDebugSphere(GetWorld(),HitBoxLocation,HitBoxRadius,16,FColor::Red,false,3);

	for (const FOverlapResult& Result:OverlapResults)
	{
		if (IsValid(Result.GetActor()))
		{
			FVector DebugLocation = Result.GetActor()->GetActorLocation();
			DebugLocation.Z += 100.f;
			DrawDebugSphere(GetWorld(),DebugLocation,30.f,10,FColor::Green,false,3);
		}
	}
}
