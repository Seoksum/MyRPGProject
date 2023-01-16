// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy_Corpse.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MyStatComponent.h"
#include "Enemies/Enemy_AnimInstance.h"
#include "Components/WidgetComponent.h"
#include "UI/EnemyHPWidget.h"
#include "Characters/Character_Parent.h"
#include "GameFrameworks/MyGameInstance.h"
#include "AI/AIController_Enemy.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Weapon_Sword.h"

AEnemy_Corpse::AEnemy_Corpse()
{
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/CityofBrass_Enemies/Meshes/Enemy/Corpse/Corpse_Sword.Corpse_Sword'"));
	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
	}
	GetMesh()->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));

	Level = 3;
	EnemyExp = 10;
	EnemyIndex = EEnemy::SkeletonEnemy;
}

void AEnemy_Corpse::BeginPlay()
{
	Super::BeginPlay();

}

void AEnemy_Corpse::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AnimInstance = Cast<UEnemy_AnimInstance>(GetMesh()->GetAnimInstance());

	if (AnimInstance)
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AEnemy_Corpse::OnAttackMontageEnded);
		AnimInstance->OnCorpseAttackHit.AddUObject(this, &AEnemy_Corpse::AttackCheck);

	}

	HpBar->InitWidget();

	auto EnemyWidget = Cast<UEnemyHPWidget>(HpBar->GetUserWidgetObject());
	if (EnemyWidget)
	{
		EnemyWidget->BindHp(Stat);
		EnemyWidget->BindLevel(Level);
	}

}

void AEnemy_Corpse::Attack()
{
	Super::Attack();

	if (IsAttacking || IsDeath)
		return;

	AnimInstance->PlayAttackMontage_Corpse();
	IsAttacking = true;
	//Stat->OnAttacking(2.f);
}

void AEnemy_Corpse::AttackCheck()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	Params.AddIgnoredActor(this);

	float AttackRange = 500.f;
	float AttackRadius = 300.f;
	FColor DrawColor;
	bool bResult = GetWorld()->LineTraceSingleByChannel(
		OUT HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * AttackRange,
		ECollisionChannel::ECC_GameTraceChannel3,
		Params);


	if (bResult)
	{
		ACharacter_Parent* Player = Cast<ACharacter_Parent>(HitResult.Actor);

		if (Player)
		{
			if (Player->bIsCrouched)
				return;
			FDamageEvent DamageEvent;
			Player->TakeDamage(Stat->GetEnemyAttack(), DamageEvent, GetController(), this);
		}
	}
}

void AEnemy_Corpse::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	OnAttackEnd.Broadcast();
}

float AEnemy_Corpse::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->OnEnemyAttacked(DamageAmount);

	FVector location = GetActorLocation();
	FVector AttackerLocation = DamageCauser->GetActorLocation();
	FRotator rotator = UKismetMathLibrary::FindLookAtRotation(location, AttackerLocation);
	//SetActorRotation(rotator);

	return DamageAmount;
}

