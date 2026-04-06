// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "CC_BaseCharacter.generated.h"


namespace CCTags
{
	extern FASTGAS_API const FName Player;
}


struct FOnAttributeChangeData;
class UAttributeSet;
class UGameplayEffect;
class UGameplayAbility;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FASCInitialized, UAbilitySystemComponent*,ASC, UAttributeSet*,AS);

UCLASS(Abstract)
class FASTGAS_API ACC_BaseCharacter : public ACharacter,public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(BlueprintAssignable)
	FASCInitialized OnASCInitialized;

	bool IsAlive() const { return bAlive; }
	void SetAlive(bool bNewAlive) { bAlive = bNewAlive; }
	
	// Sets default values for this character's properties
	ACC_BaseCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual UAttributeSet* GetAttributeSet() const;


	UFUNCTION(BlueprintCallable)
	virtual void HandleRespawn();

	UFUNCTION(BlueprintCallable)
	void ResetAttributes();

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GiveStartupAbilities();
	void InitializeAttributes() const;

	void OnHealthChanged(const FOnAttributeChangeData& Data);

	virtual void HandleDeath();

	void BindHealthChangedDelegate();
private:
	UPROPERTY(EditDefaultsOnly,Category = "CC|Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	UPROPERTY(EditDefaultsOnly,Category = "CC|Effects")
	TSubclassOf<UGameplayEffect> InitializeAttributeEffect;

	UPROPERTY(EditDefaultsOnly,Category = "CC|Effects")
	TSubclassOf<UGameplayEffect> ResetAttributeEffect;


	//私有变量蓝图只读 需要添加 meta = (AllowPrivateAccess = "true")
	UPROPERTY(blueprintreadonly, meta = (AllowPrivateAccess = "true"),Replicated)
	bool bAlive = true;
};
