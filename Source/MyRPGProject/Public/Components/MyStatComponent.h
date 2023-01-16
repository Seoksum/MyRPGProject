// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpChanged);
DECLARE_MULTICAST_DELEGATE(FOnManaChanged);
DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE(FOnPlayerLevelUp);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYRPGPROJECT_API UMyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMyStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;


public:

	void SetLevel(int32 NewLevel);
	void SetPlayerLevel(int32 NewLevel);

	void OnAttacked(float DamageAmount);
	void OnAttacking(float ManaAmount);

	void UseHpPotion(float Amount);
	void UseManaPotion(float Amount);

	void SetEnemyHp(int32 NewHp);
	void OnEnemyAttacked(float DamageAmount);

	int32 GetEnemyHp() { return EnemyHp; }
	float GetEnemyHpRatio() { return EnemyHp / (float)EnemyMaxHp; }

	void SetHp(int32 NewHp);
	int32 GetHp() { return Hp; }
	int32 GetMaxHp() { return MaxHp; }
	float GetHpRatio() { return Hp / (float)MaxHp; }

	void SetMana(int32 NewMana);
	int32 GetMana() { return Mana; }
	int32 GetMaxMana() { return MaxMana; }
	float GetManaRatio() { return Mana / (float)MaxMana; }

	int32 GetAttack() { return Attack; }
	int32 GetAttack_Q() { return Attack_Q; }
	int32 GetAttack_E() { return Attack_E; }
	int32 GetAttack_R() { return Attack_R; }

	int32 GetEnemyAttack() { return EnemyAttack; }

	void SetExp(int32 NewExp);
	int32 GetExp() { return Exp; }



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

private:
	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 EnemyHp;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 EnemyMaxHp;

	UPROPERTY(EditAnywhere, Category = Stat, Meta = (AllowPrivateAccess = true))
	int32 EnemyAttack;

public:

	UPROPERTY(EditAnywhere, Meta = (AllowPrivateAccess = true))
	class UMyGameInstance* MyGameInstance;

	FOnHpChanged OnHpChanged;

	FOnManaChanged OnManaChanged;

	FOnDeath OnDeath;

	FOnPlayerLevelUp OnPlayerLevelUp;

	UPROPERTY(EditAnywhere)
	int32 NowHp;

	UPROPERTY(EditAnywhere)
		int32 NowExp;



		
};
