// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "CC_BaseCharacter.generated.h"

class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FASCInitialized, UAbilitySystemComponent*,ASC, UAttributeSet*,AS);

UCLASS(Abstract)
class FASTGAS_API ACC_BaseCharacter : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FASCInitialized OnASCInitialized;
	
	// Sets default values for this character's properties
	ACC_BaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual UAttributeSet* GetAttributeSet() const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GiveStartupAbilities();
	void InitializeAttributes() const;
	
private:
	UPROPERTY(EditDefaultsOnly,Category = "CC|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly,Category = "CC|Effects")
	TSubclassOf<UGameplayEffect> InitializeAttributeEffect;
};
