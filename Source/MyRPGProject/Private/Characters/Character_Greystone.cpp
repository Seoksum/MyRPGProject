// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Character_Greystone.h"
#include "Characters/Character_Parent.h"
#include "Characters/AnimInstance_Greystone.h"
#include "GameFrameworks/MyPlayerController.h"
#include "GameFrameworks/MyGameInstance.h"
#include "UI/InGame.h"
#include "Components/TextBlock.h"
#include "Components/MyStatComponent.h"
#include "Components/InventoryComponent.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "UI/HPWidget.h"
#include "Weapon_Gun.h"
#include "Weapon_Sword.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"
#include "Enemies/Enemy.h"


ACharacter_Greystone::ACharacter_Greystone()
{
	if (MyPlayerController)
	{
		UInGame* MyHUD = Cast<UInGame>(MyPlayerController->CurrentWidget);
		if (MyHUD)
		{
			MyHUD->SetGreystone();
		}
	}

	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	Mana = 5;
	LandYsize = 1.5f;
}

void ACharacter_Greystone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Attack_Q"), EInputEvent::IE_Pressed, this, &ACharacter_Greystone::AttackQ_Implementation);
	PlayerInputComponent->BindAction(TEXT("Attack_E"), EInputEvent::IE_Pressed, this, &ACharacter_Greystone::AttackE_Implementation);
	PlayerInputComponent->BindAction(TEXT("Attack_R"), EInputEvent::IE_Pressed, this, &ACharacter_Greystone::AttackR_Implementation);
}

void ACharacter_Greystone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 벽 위를 더 이상 오를 수 없어 착지해야할 때
	if (bIsClimbingComplete)
	{
		SetActorLocation(GetActorLocation() + GetActorForwardVector() * 3.f);
		SetActorLocation(GetActorLocation() + GetActorUpVector() * 7.f);
	}

	//if (AttackMoving)
	//{
	//	FVector NewLocation1 = GetActorLocation() + GetActorForwardVector() * 7.f;
	//	SetActorLocation(NewLocation1);
	//}
}

void ACharacter_Greystone::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->HideBoneByName(TEXT("sword_bottom"), EPhysBodyOp::PBO_None);


}

void ACharacter_Greystone::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (MyGameInstanceRef)
	{
		Level = MyGameInstanceRef->GetNowLevel();
		Stat->SetPlayerLevel(Level);
	}

	AnimInstance = Cast<UAnimInstance_Greystone>(GetMesh()->GetAnimInstance());
	if (AnimInstance)
	{
		AnimInstance->OnAttackHit_Q.AddUObject(this, &ACharacter_Greystone::SkillAttackCheck);
		AnimInstance->OnAttackHit_E.AddUObject(this, &ACharacter_Greystone::SkillAttackCheck);
		AnimInstance->OnAttackHit_R.AddUObject(this, &ACharacter_Greystone::SkillAttackCheck);

		AnimInstance->OnMontageEnded.AddDynamic(this, &ACharacter_Greystone::OnAttackMontageEnded);
		AnimInstance->OnCameraShake.AddUObject(this, &ACharacter_Greystone::CameraShakeCheck);
		
	}
}

void ACharacter_Greystone::Attack()
{
	Super::Attack();

	if (IsAttacking)
		return;

	if (CurrentWeaponIndex == EWeapon::Sword)
	{
		AnimInstance->PlayAttackMontage();
		AnimInstance->JumpToSection(AttackIndex);

		AttackIndex = (AttackIndex + 1) % TotalAttackIndex;
		IsAttacking = true;
	}

	else if (CurrentWeaponIndex == EWeapon::Bow)
	{
		AnimInstance->PlayBowAttackMontage();
		IsAttacking = true;
	}

}

void ACharacter_Greystone::AttackQ_Implementation()
{
	int32 ManaQ = Mana;
	if (CurrentWeaponIndex != EWeapon::Sword || IsAttackingQ || Stat->GetMana()- ManaQ <= 0.f)
		return;

	IsAttackingQ = true;
	AnimInstance->PlayAttackMontageQ();
	Stat->OnAttacking(ManaQ); //Mana 크기만큼 플레이어의 마나 소모
	Remaining_SkillQ = 3;
	GetWorldTimerManager().SetTimer(QSkillHandle, this, &ACharacter_Greystone::EndAttack_Q, 1.f, true);
}

void ACharacter_Greystone::AttackE_Implementation()
{
	int32 ManaE = Mana + 5;
	if (CurrentWeaponIndex != EWeapon::Sword || IsAttackingE || Stat->GetMana() - ManaE <= 0.f)
		return;

	IsAttackingE = true;
	AnimInstance->PlayAttackMontageE();
	Remaining_SkillE = 6;
	Stat->OnAttacking(ManaE);
	GetWorldTimerManager().SetTimer(ESkillHandle, this, &ACharacter_Greystone::EndAttack_E, 1.f, true);
}

void ACharacter_Greystone::AttackR_Implementation()
{
	int32 ManaR = Mana + 10;
	if (CurrentWeaponIndex != EWeapon::Sword || IsAttackingR || Stat->GetMana()-ManaR <= 0.f)
		return;

	IsAttackingR = true;
	AttackMoving = true;
	Remaining_SkillR = 10;
	
	AnimInstance->PlayAttackMontageR();

	Stat->OnAttacking(ManaR);
	GetWorldTimerManager().SetTimer(RSkillHandle, this, &ACharacter_Greystone::EndAttack_R, 1.f, true);
}

void ACharacter_Greystone::SkillAttackCheck(int32 damage, float TraceDistance, class UParticleSystem* Particle)
{
	if (Sword) { Sword->Attack(damage, TraceDistance, Particle); }
}

void ACharacter_Greystone::PressClimbingUp()
{
	Super::PressClimbingUp();
	if (!bIsOnWall && bIsClimbingComplete)
	{
		AnimInstance->PlayClimbingComplete();
	}
}

void ACharacter_Greystone::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	IsAttacking = false;
	AttackMoving = false;
}

void ACharacter_Greystone::CameraShakeCheck()
{
	Super::CameraShakeCheck();
	AttackMoving = false;
}
