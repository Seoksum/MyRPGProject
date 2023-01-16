// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGame.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "GameFrameworks/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MyStatComponent.h"
#include "Characters/Character_Greystone.h"


void UInGame::SetGreystone()
{
	SetLevel();

	// Greystone의 스킬 썸네일 지정하기
	const FString Q_Path = "Texture2D'/Game/Images/Greystone_Q.Greystone_Q'";
	const FString E_Path = "Texture2D'/Game/Images/Greystone_E.Greystone_E'";
	const FString R_Path = "Texture2D'/Game/Images/Greystone_R.Greystone_R'";
	
	SetCharacterSkillThumbnail(Q_Path, E_Path, R_Path);
}

void UInGame::SetCountess()
{
	SetLevel();

	// Countess의 스킬 썸네일 지정하기
	const FString Q_Path = "Texture2D'/Game/Images/Countess_Q.Countess_Q'";
	const FString E_Path = "Texture2D'/Game/Images/Countess_E.Countess_E'";
	const FString R_Path = "Texture2D'/Game/Images/Countess_R.Countess_R'";
	
	SetCharacterSkillThumbnail(Q_Path, E_Path, R_Path);
}

void UInGame::SetCharacterSkillThumbnail(FString Q_Path, FString E_Path, FString R_Path)
{
	UTexture2D* MyTexture_Q = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *Q_Path));
	UTexture2D* MyTexture_E = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *E_Path));
	UTexture2D* MyTexture_R = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), nullptr, *R_Path));

	Image_Q->SetBrushFromTexture(MyTexture_Q);
	Image_E->SetBrushFromTexture(MyTexture_E);
	Image_R->SetBrushFromTexture(MyTexture_R);
}

void UInGame::SetLevel()
{
	// 현재 레벨을 우측 상단에 표시 
	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstanceRef)
	{
		const FString MyLevel = FString::Printf(TEXT("%d"), MyGameInstanceRef->GetNowLevel());
		Level->SetText(FText::FromString(MyLevel));
	}
}

void UInGame::BindLevel(UMyStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	CurrentStatComp->OnPlayerLevelUp.AddUObject(this, &UInGame::SetLevel);
}

void UInGame::BindHp(UMyStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	CurrentStatComp->OnHpChanged.AddUObject(this, &UInGame::UpdateHp);
}

void UInGame::UpdateHp()
{
	if (CurrentStatComp.IsValid())
	{
		PB_HP->SetPercent(CurrentStatComp->GetHpRatio());
	}
}

void UInGame::BindMana(UMyStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	CurrentStatComp->OnManaChanged.AddUObject(this, &UInGame::UpdateMana);
}

void UInGame::UpdateMana()
{
	if (CurrentStatComp.IsValid())
	{
		PB_Mana->SetPercent(CurrentStatComp->GetManaRatio());
	}
}