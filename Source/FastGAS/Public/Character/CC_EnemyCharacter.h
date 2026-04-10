// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CC_BaseCharacter.h"
#include "CC_EnemyCharacter.generated.h"

class UAttributeSet;

UCLASS()
class FASTGAS_API ACC_EnemyCharacter : public ACC_BaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACC_EnemyCharacter();

	virtual UAttributeSet* GetAttributeSet() const override;


	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "CC|AI")
	float AcceptanceRadius{500.f};
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "CC|AI")
	float MinAttackDelay{0.1f};
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = "CC|AI")
	float MaxAttackDelay{0.5f};
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void HandleDeath() override;

public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
private:

	UPROPERTY(VisibleAnywhere,Category = "CC")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
