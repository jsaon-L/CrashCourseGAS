// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CC_Projectile.generated.h"

class UGameplayEffect;
class UProjectileMovementComponent;

UCLASS()
class FASTGAS_API ACC_Projectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACC_Projectile();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "CC|Projectile",meta = (ExposeOnSpawn,ClampMin = "0.0"))
	float Damage{10.f};

	
	UFUNCTION(BlueprintImplementableEvent)
	void SpawnImpactEffects();
private:

	UPROPERTY(VisibleAnywhere, Category="CC|Projectile")
	TObjectPtr<UProjectileMovementComponent> ProjectileComponent;

	UPROPERTY(EditDefaultsOnly, Category="CC|Projectile")
	TSubclassOf<UGameplayEffect> DamageEffect;


};
