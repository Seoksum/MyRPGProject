// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */

UENUM()
enum ECharacterIndex
{
	Greystone,
	Countess,
};

UENUM()
enum EWeapon
{
	Sword,
	Gun,
	Bow,
};


UENUM()
enum EEnemy
{
	SkeletonEnemy,
	BossEnemy,
};


USTRUCT()
struct FCharacterData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Attack_Q;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Attack_E;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Attack_R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Exp;

	// ������ ���̺� 
};


UCLASS()
class MYRPGPROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:

	UMyGameInstance();

	virtual void Init() override;

	FCharacterData* GetStatData(int32 Level);

	// ĳ���� Ÿ�� ����
	void SetCharacterTypeIndex(int32 Index);
	int32 GetCharacterTypeIndex();

	// ĳ���� �޽� ����
	void SetCharacterMeshIndex(int32 Index);
	int32 GetCharacterMeshIndex();
	
	// ���� �� or ���� ����۽� ���� �缳���� ���� �Լ�  
	void SetNowLevel(int32 Level);
	int32 GetNowLevel();

	// ü�� ���� or ���� ���� HP �缳���� ���� �Լ�  
	void SetNowHp(int32 HP);
	int32 GetNowHp();

	// ��ų ��� or ���� ���� Mana �缳���� ���� �Լ�  
	void SetNowMana(int32 Mana);
	int32 GetNowMana();

	// Respawn�� �� �ִ� Ƚ���� �����ϴ� �Լ�
	void SetRemainingCount(int32 Count);
	int32 GetRemainingCount();

	// IsRestarting�Ͻ� �÷��̾��� ����,ü���� �ʱ�ȭ �˴ϴ�. 
	UPROPERTY(VisibleAnywhere)
	bool IsRestarting = false;


private:
	UPROPERTY()
	class UDataTable* MyStatTable;

	UPROPERTY(VisibleAnywhere)
	int32 CharacterTypeIdx;

	UPROPERTY(VisibleAnywhere)
	int32 CharacterMeshIdx;

	UPROPERTY(VisibleAnywhere)
	int32 NowHp;

	UPROPERTY(VisibleAnywhere)
	int32 NowMana;

	UPROPERTY(EditAnywhere)
	int32 NowLevel;

	UPROPERTY(EditAnywhere)
	int32 RemainingCount;


};
