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

	// 데이터 테이블 
};


UCLASS()
class MYRPGPROJECT_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	

public:

	UMyGameInstance();

	virtual void Init() override;

	FCharacterData* GetStatData(int32 Level);

	// 캐릭터 타입 선택
	void SetCharacterTypeIndex(int32 Index);
	int32 GetCharacterTypeIndex();

	// 캐릭터 메시 선택
	void SetCharacterMeshIndex(int32 Index);
	int32 GetCharacterMeshIndex();
	
	// 레벨 업 or 게임 재시작시 레벨 재설정을 위한 함수  
	void SetNowLevel(int32 Level);
	int32 GetNowLevel();

	// 체력 감소 or 포션 사용시 HP 재설정을 위한 함수  
	void SetNowHp(int32 HP);
	int32 GetNowHp();

	// 스킬 사용 or 포션 사용시 Mana 재설정을 위한 함수  
	void SetNowMana(int32 Mana);
	int32 GetNowMana();

	// Respawn할 수 있는 횟수를 관리하는 함수
	void SetRemainingCount(int32 Count);
	int32 GetRemainingCount();

	// IsRestarting일시 플레이어의 레벨,체력은 초기화 됩니다. 
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
