// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enemy_AnimInstance.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnBossAttackHit);
DECLARE_MULTICAST_DELEGATE(FOnCorpseAttackHit);
DECLARE_MULTICAST_DELEGATE(FOnThrowProjectile);
DECLARE_MULTICAST_DELEGATE(FOnSkillAttack);


UCLASS()
class MYRPGPROJECT_API UEnemy_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UEnemy_AnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	
	void BSNode();

	void PlayAttackMontage_Boss();
	void PlayAttackMontage_Corpse();
	void PlayAttackMontage_Thrower();


	void JumpToSection(int32 SectionIndex);
	FName GetAttackMontageName(int32 SectionIndex);

private:

	UFUNCTION()
	void AnimNotify_BossAttackHit();

	UFUNCTION()
	void AnimNotify_AttackHitCorpse();

	UFUNCTION()
	void AnimNotify_ThrowProjectile();

	UFUNCTION()
	void AnimNotify_SkillHit();


private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,  Meta = (AllowPrivateAccess = true))
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,  Meta = (AllowPrivateAccess = true))
	float SpeedSide;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,  Meta = (AllowPrivateAccess = true))
	float SpeedForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool IsDeath;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = true))
	bool IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Corpse", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Corpse_AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Thrower", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Thrower_AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss", Meta = (AllowPrivateAccess = true))
	UAnimMontage* Boss_AttackMontage;




public:
	FOnBossAttackHit OnBossAttackHit;

	FOnCorpseAttackHit OnCorpseAttackHit;

	FOnThrowProjectile OnThrowProjectile;

	FOnSkillAttack OnSkillAttack;

};
