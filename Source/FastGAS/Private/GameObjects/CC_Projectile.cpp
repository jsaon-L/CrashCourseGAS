// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/CC_Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Character/CC_PlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayTags/CCTags.h"
#include "Utils/CC_BlueprintLibrary.h"


// Sets default values
ACC_Projectile::ACC_Projectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	bReplicates = true;
}

void ACC_Projectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!HasAuthority()) return;
	
	ACC_PlayerCharacter* PlayerCharacter = Cast<ACC_PlayerCharacter>(OtherActor);
	if (!PlayerCharacter) return;
	if (!PlayerCharacter->IsAlive()) return;

	UAbilitySystemComponent* AbilitySystemComponent = PlayerCharacter->GetAbilitySystemComponent();
	if (!AbilitySystemComponent) return;

	if (!IsValid(DamageEffect)) return;

	FGameplayEventData Payload;
	Payload.Instigator = GetOwner();
	Payload.Target = PlayerCharacter;

	UCC_BlueprintLibrary::SendDamageEventToPlayer(PlayerCharacter,DamageEffect,Payload,CCTags::SetByCaller::Projectile,Damage);
	
	SpawnImpactEffects();
	Destroy();
}

