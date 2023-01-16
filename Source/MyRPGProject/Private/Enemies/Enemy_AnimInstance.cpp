// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy_AnimInstance.h"
#include "Enemies/Enemy.h"
#include "GameFramework/PawnMovementComponent.h"



UEnemy_AnimInstance::UEnemy_AnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM (TEXT("AnimMontage'/Game/Enemies/Animations/Boss_AttackMontage.Boss_AttackMontage'"));
	if (AM.Succeeded())
	{
		Boss_AttackMontage = AM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Corpse(TEXT("AnimMontage'/Game/Enemies/Animations/Corpse_AttackMontage.Corpse_AttackMontage'"));
	if (AM_Corpse.Succeeded())
	{
		Corpse_AttackMontage = AM_Corpse.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM_Thrower(TEXT("AnimMontage'/Game/Enemies/Animations/Corpse_ThrowMontage.Corpse_ThrowMontage'"));
	if (AM_Thrower.Succeeded())
	{
		Thrower_AttackMontage = AM_Thrower.Object;
	}


}

void UEnemy_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	BSNode();
}

void UEnemy_AnimInstance::BSNode()
{
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Speed = Pawn->GetVelocity().Size();
		auto Enemy = Cast<AEnemy>(Pawn);
		if (Enemy)
		{
			SpeedSide = Enemy->GetActorRightVector().Size();
			SpeedForward = Enemy->GetActorForwardVector().Size();

			IsFalling = Enemy->GetMovementComponent()->IsFalling();
			IsDeath = Enemy->IsDeath;
			IsAttacking = Enemy->IsAttacking;
		}
	}
}

void UEnemy_AnimInstance::PlayAttackMontage_Boss()
{
	Montage_Play(Boss_AttackMontage, 1.f);
}

void UEnemy_AnimInstance::PlayAttackMontage_Corpse()
{
	Montage_Play(Corpse_AttackMontage, 1.f);
}

void UEnemy_AnimInstance::PlayAttackMontage_Thrower()
{
	Montage_Play(Thrower_AttackMontage, 1.f);
}

void UEnemy_AnimInstance::JumpToSection(int32 SectionIndex)
{
	FName Name = GetAttackMontageName(SectionIndex);
	Montage_JumpToSection(Name, Boss_AttackMontage);
}

FName UEnemy_AnimInstance::GetAttackMontageName(int32 SectionIndex)
{
	return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
}

void UEnemy_AnimInstance::AnimNotify_BossAttackHit()
{
	OnBossAttackHit.Broadcast();
}

void UEnemy_AnimInstance::AnimNotify_AttackHitCorpse()
{
	OnCorpseAttackHit.Broadcast();
}

void UEnemy_AnimInstance::AnimNotify_ThrowProjectile()
{
	OnThrowProjectile.Broadcast();
}

void UEnemy_AnimInstance::AnimNotify_SkillHit()
{
	OnSkillAttack.Broadcast();
}