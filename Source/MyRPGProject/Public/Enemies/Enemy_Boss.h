// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemies/Enemy.h"
#include "Enemy_Boss.generated.h"

/**
 * 
 */



UCLASS()
class MYRPGPROJECT_API AEnemy_Boss : public AEnemy
{
	GENERATED_BODY()
	

public:

	AEnemy_Boss();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

protected:
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	virtual void Attack() override;
	
	void AttackCheck();

	void SkillAttack();

	virtual void Landed(const FHitResult& Hit) override;


public:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particle")
	class UParticleSystem* SkillEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Particle")
		class UParticleSystem* JumpEffect;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool JumpAttacking;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		TSubclassOf<UCameraShakeBase> AttackedCamShake;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	TSubclassOf<class AWeapon_Sword> LeftSwordClass;

	UPROPERTY()
	class AWeapon_Sword* LeftSword;

	UPROPERTY()
	int32 AttackIndex = 0;

	

};
