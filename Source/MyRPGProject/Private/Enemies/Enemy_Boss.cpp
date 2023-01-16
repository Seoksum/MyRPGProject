// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy_Boss.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MyStatComponent.h"
#include "Enemies/Enemy_AnimInstance.h"
#include "Components/WidgetComponent.h"
#include "UI/EnemyHPWidget.h"
#include "Characters/Character_Parent.h"
#include "GameFrameworks/MyGameInstance.h"
#include "AI/AIController_Enemy.h"
#include "DrawDebugHelpers.h"
#include "Weapon_Sword.h"
#include "AI/AIController_Enemy.h"


AEnemy_Boss::AEnemy_Boss()
{
	Level = 5;
	EnemyExp = 100;
	EnemyIndex = EEnemy::BossEnemy;
}

void AEnemy_Boss::BeginPlay()
{
	Super::BeginPlay();

	LeftSword = GetWorld()->SpawnActor<AWeapon_Sword>(LeftSwordClass);
	if (LeftSword)
	{
		LeftSword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("LeftSwordSocket"));
		LeftSword->SetOwner(this);
	}

}

void AEnemy_Boss::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	//Stat->SetLevel(Level);
	AnimInstance = Cast<UEnemy_AnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AEnemy_Boss::OnAttackMontageEnded);
		AnimInstance->OnBossAttackHit.AddUObject(this, &AEnemy_Boss::AttackCheck);
		AnimInstance->OnSkillAttack.AddUObject(this, &AEnemy_Boss::SkillAttack);

	}

	HpBar->InitWidget();

	auto EnemyWidget = Cast<UEnemyHPWidget>(HpBar->GetUserWidgetObject());
	if (EnemyWidget)
	{
		EnemyWidget->BindHp(Stat);
		EnemyWidget->BindLevel(Level);
	}
	
}

void AEnemy_Boss::Attack()
{
	Super::Attack();

	if (IsAttacking)
		return;

	IsAttacking = true;

	if (Stat->GetEnemyHpRatio() < 0.5f && !JumpAttacking)
	{
		Jump();
		FVector Location = GetActorLocation();
		Location.Z += 200.f;
		SetActorLocation(Location);
		JumpAttacking = true;
	}

	AnimInstance->PlayAttackMontage_Boss();
	AnimInstance->JumpToSection(AttackIndex);
	AttackIndex = (AttackIndex + 1) % 4;
}

void AEnemy_Boss::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 500.f;
	float AttackRadius = 150.f;

	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(AttackRadius),
		Params);

	if (bResult)
	{
		ACharacter_Parent* Player = Cast<ACharacter_Parent>(HitResult.Actor);
		if (Player)
		{
			FDamageEvent DamageEvent;
			Player->TakeDamage(Stat->GetEnemyAttack(), DamageEvent, GetController(), this);
		}
	}
}

void AEnemy_Boss::SkillAttack()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	
	bool bResult = GetWorld()->SweepSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 1500.f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel3,
		FCollisionShape::MakeSphere(100.f),
		Params);

	if (bResult)
	{
		ACharacter_Parent* Player = Cast<ACharacter_Parent>(HitResult.Actor);
		if (Player)
		{
			FDamageEvent DamageEvent;
			Player->TakeDamage(Stat->GetEnemyAttack(), DamageEvent, GetController(), this);
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SkillEffect, Player->GetTransform());
		}
	}

}

void AEnemy_Boss::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	FTransform Transform(FRotator::ZeroRotator, Hit.ImpactPoint,FVector(10.f,10.f,10.f));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), JumpEffect, Transform);

	if (AttackedCamShake)
	{
		OnBossJump.Broadcast(AttackedCamShake);
	}
}

void AEnemy_Boss::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

float AEnemy_Boss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->OnEnemyAttacked(DamageAmount);

	return DamageAmount;
}
