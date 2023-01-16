// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameworks/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MyStatComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/AudioComponent.h"
#include "Items/Item.h"



UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DATA(TEXT("DataTable'/Game/Data/StatTable.StatTable'"));
	MyStatTable = DATA.Object;

	CharacterTypeIdx = 0;
	CharacterMeshIdx = 0;
	IsRestarting = false;
	RemainingCount = 3;
}

void UMyGameInstance::Init()
{
	Super::Init();

	NowLevel = 2;
	NowHp = GetStatData(NowLevel)->MaxHp;
	NowMana = GetStatData(NowLevel)->MaxMana;

}

FCharacterData* UMyGameInstance::GetStatData(int32 Level)
{
	return MyStatTable->FindRow<FCharacterData>(*FString::FromInt(Level), TEXT(""));
}

void UMyGameInstance::SetCharacterTypeIndex(int32 Index)
{
	CharacterTypeIdx = Index;
}

int32 UMyGameInstance::GetCharacterTypeIndex()
{
	return CharacterTypeIdx;
}

void UMyGameInstance::SetCharacterMeshIndex(int32 Index)
{
	CharacterMeshIdx = Index;
}

int32 UMyGameInstance::GetCharacterMeshIndex()
{
	return CharacterMeshIdx;
}

void UMyGameInstance::SetNowLevel(int32 Level)
{
	NowLevel = Level;
	IsRestarting = true;
}

int32 UMyGameInstance::GetNowLevel()
{
	return NowLevel;
}

void UMyGameInstance::SetNowHp(int32 HP)
{
	NowHp = HP;
}

int32 UMyGameInstance::GetNowHp()
{
	return NowHp;
}

void UMyGameInstance::SetNowMana(int32 Mana)
{
	NowMana = Mana;
}

int32 UMyGameInstance::GetNowMana()
{
	return NowMana;
}

void UMyGameInstance::SetRemainingCount(int32 Count)
{
	RemainingCount = Count;
}

int32 UMyGameInstance::GetRemainingCount()
{
	return RemainingCount;
}


