// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HPWidget.h"
#include "Components/ProgressBar.h"
#include "Components/MyStatComponent.h"


void UHPWidget::BindHp(class UMyStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	CurrentStatComp->OnHpChanged.AddUObject(this, &UHPWidget::UpdateHp);
	// 플레이어의 HP 변화시 ProgressBar에 업데이트 해주어야 함.
}

void UHPWidget::UpdateHp()
{
	if (CurrentStatComp.IsValid())
		PB_HpBar->SetPercent(CurrentStatComp->GetHpRatio());
}

void UHPWidget::BindMana(class UMyStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	CurrentStatComp->OnManaChanged.AddUObject(this, &UHPWidget::UpdateMana);
	// 플레이어의 Mana 변화시 ProgressBar에 업데이트 해주어야 함.
}

void UHPWidget::UpdateMana()
{
	if (CurrentStatComp.IsValid())
		PB_ManaBar->SetPercent(CurrentStatComp->GetManaRatio());
}
