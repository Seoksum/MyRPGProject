// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Enemy.h"
#include "Enemy_Corpse.generated.h"

/**
 * 
 */


UCLASS()
class MYRPGPROJECT_API AEnemy_Corpse : public AEnemy
{
	GENERATED_BODY()

public:

	AEnemy_Corpse();

	//virtual void OnDeath_Implementation() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

public:

	virtual void Attack() override;

	void AttackCheck();

	
};
