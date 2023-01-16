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

// Called when the game starts
void UMyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}

void UMyStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void UMyStatComponent::SetLevel(int32 NewLevel)
{
	MyGameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstance)
	{
		auto StatData = MyGameInstance->GetStatData(NewLevel);

		if (StatData)
		{
			Level = StatData->Level;
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

			OnHpChanged.Broadcast();
			OnManaChanged.Broadcast();

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
	MyGameInstance->SetNowMana(NewMana);

	if (Mana < 0)
	{
		Mana = 0;
	}
	OnManaChanged.Broadcast();
}

void UMyStatComponent::SetExp(int32 NewExp)
{
	Exp += NewExp;
	auto StatData = MyGameInstance->GetStatData(Level);
	int32 LevelExp = StatData->Exp;

	if (LevelExp <= Exp)
	{
		OnPlayerLevelUp.Broadcast();
		Exp -= LevelExp;
		UE_LOG(LogTemp, Log, TEXT("Now Exp : %d , BeforeLevelExp : %d"), Exp, LevelExp);
	}
}

void UMyStatComponent::OnAttacked(float DamageAmount)
{
	int32 NewHp = Hp -= DamageAmount;
	SetHp(NewHp);
}

void UMyStatComponent::OnAttacking(float ManaAmount)
{
	if (Mana < 0)
	{
		Mana = 0;
		return;
	}
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



