// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpChanged);
DECLARE_MULTICAST_DELEGATE(FOnManaChanged);
DECLARE_MULTICAST_DELEGATE(FOnPlayerLevelUp);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYRPGPROJECT_API UMyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyStatComponent();

protected:

	virtual void InitializeComponent() override;

public:

	void SetEnemyLevel(int32 NewLevel);
	void SetPlayerLevel(int32 NewLevel);

	void OnAttacked(float DamageAmount);// 공격 받으면 HP 감소
	void OnAttacking(float ManaAmount);	// 공격 하면 Mana 감소

	void UseHpPotion(float Amount);		// Hp 포션 아이템 클릭시 HP 증가
	void UseManaPotion(float Amount);	// Mana 포션 아이템 클릭시 Mana 증가

	void SetEnemyHp(int32 NewHp);		// 적 HP 델레게이트 호출
	void OnEnemyAttacked(float DamageAmount);	// 적이 공격 받을 때 HP 감소

	//int32 GetEnemyHp() { return EnemyHp; }
	float GetEnemyHpRatio() { return EnemyHp / (float)EnemyMaxHp; }

	void SetHp(int32 NewHp);
	float GetHpRatio() { return Hp / (float)MaxHp; }

	void SetMana(int32 NewMana);
	int32 GetMana() { return Mana; }
	float GetManaRatio() { return Mana / (float)MaxMana; }

	int32 GetAttack() { return Attack; }
	int32 GetAttack_Q() { return Attack_Q; }
	int32 GetAttack_E() { return Attack_E; }
	int32 GetAttack_R() { return Attack_R; }

	int32 GetEnemyAttack() { return EnemyAttack; }

	void SetExp(int32 NewExp);


private:

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Level;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Hp;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 MaxHp;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Mana;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 MaxMana;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Attack;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Attack_Q;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Attack_E;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Attack_R;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 Exp;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 EnemyHp;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 EnemyMaxHp;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 EnemyAttack;

public:

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	class UMyGameInstance* MyGameInstance;

	UPROPERTY(EditAnywhere)
	int32 NowHp;

	UPROPERTY(EditAnywhere)
	int32 NowExp;

	FOnHpChanged OnHpChanged;

	FOnManaChanged OnManaChanged;

	FOnPlayerLevelUp OnPlayerLevelUp;
		
};
