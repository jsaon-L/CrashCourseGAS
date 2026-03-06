// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "CC_BaseCharacter.generated.h"

class UGameplayAbility;

UCLASS(Abstract)
class FASTGAS_API ACC_BaseCharacter : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACC_BaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GiveStartupAbilities();
	
private:
	UPROPERTY(EditDefaultsOnly,Category = "CC|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

};
