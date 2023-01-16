// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MyStatComponent.h"
#include "GameFrameworks/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Interface/HealthInterface.h"
#include "GameFramework/Actor.h"
#include "Characters/Character_Greystone.h"

// Sets default values for this component's properties
UMyStatComponent::UMyStatComponent()
{
	bWantsInitializeComponent = true;

}

void UMyStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void UMyStatComponent::SetEnemyLevel(int32 NewLevel)
{
	MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance)
	{
		auto StatData = MyGameInstance->GetStatData(NewLevel);
		if (StatData)
		{
			Level = NewLevel;
			SetEnemyHp(StatData->MaxHp);
			EnemyMaxHp = StatData->MaxHp;
			EnemyAttack = StatData->Attack;
		}
	}
}

void UMyStatComponent::SetPlayerLevel(int32 NewLevel)
{
	MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	bool IsRestarting = MyGameInstance->IsRestarting;
	Level = NewLevel;

	if (MyGameInstance)
	{
		auto StatData = MyGameInstance->GetStatData(Level);

		if (StatData)
		{
			//Level = StatData->Level;
			if (IsRestarting)
			{
				Hp = StatData->MaxHp;
				Mana = StatData->MaxMana;
				MyGameInstance->IsRestarting = false;
			}
			else
			{
				Hp = MyGameInstance->GetNowHp();
				Mana = MyGameInstance->GetNowMana();
			}
			SetHp(Hp);
			SetMana(Mana);

			MaxHp = StatData->MaxHp;
			MaxMana = StatData->MaxMana;

			MyGameInstance->SetNowHp(Hp);
			MyGameInstance->SetNowMana(Mana);

			Attack = StatData->Attack;
			Attack_Q = StatData->Attack_Q;
			Attack_E = StatData->Attack_E;
			Attack_R = StatData->Attack_R;
		}
	}
}


void UMyStatComponent::SetHp(int32 NewHp)
{
	Hp = NewHp;
	MyGameInstance->SetNowHp(NewHp);

	if (Hp <= 0)
	{
		Hp = 0;
		if (GetOwner()->Implements<UHealthInterface>())
		{
			IHealthInterface::Execute_OnDeath(GetOwner());
		}
	}
	OnHpChanged.Broadcast();
}

void UMyStatComponent::SetMana(int32 NewMana)
{
	Mana = NewMana;
	if (Mana < 0) { Mana = 0; }
	MyGameInstance->SetNowMana(Mana);
	OnManaChanged.Broadcast();
}

void UMyStatComponent::SetExp(int32 NewExp)
{
	Exp += NewExp;
	auto StatData = MyGameInstance->GetStatData(Level);
	int32 LevelExp = StatData->Exp; // 레벨업을 위해 얻어야하는 경험치 값

	if (LevelExp <= Exp) // 레벨 경험치를 넘기면
	{
		OnPlayerLevelUp.Broadcast(); // 레벨업 시켜주는 델레게이트 
		Exp -= LevelExp; // 남은 경험치는 이월 
	}
}

void UMyStatComponent::OnAttacked(float DamageAmount)
{
	int32 NewHp = Hp -= DamageAmount;
	SetHp(NewHp);
}

void UMyStatComponent::OnAttacking(float ManaAmount)
{
	int32 NewMana = Mana -= ManaAmount;
	SetMana(NewMana);
}

void UMyStatComponent::UseHpPotion(float Amount)
{
	int NewHp = Hp += Amount;
	SetHp(NewHp);
}

void UMyStatComponent::UseManaPotion(float Amount)
{
	int NewMana = Mana += Amount;
	if (NewMana >= MaxMana)
		NewMana = MaxMana;
	SetMana(NewMana);
}

void UMyStatComponent::SetEnemyHp(int32 NewHp)
{
	EnemyHp = NewHp;

	if (EnemyHp <= 0)
	{
		EnemyHp = 0;
		if (GetOwner()->Implements<UHealthInterface>())
		{
			IHealthInterface::Execute_OnDeath(GetOwner());
		}
	}
	OnHpChanged.Broadcast();
}

void UMyStatComponent::OnEnemyAttacked(float DamageAmount)
{
	int NewEnemyHp = EnemyHp -= DamageAmount;
	SetEnemyHp(NewEnemyHp);
}



