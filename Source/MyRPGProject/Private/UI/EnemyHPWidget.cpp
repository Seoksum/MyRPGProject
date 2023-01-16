// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyHPWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/MyStatComponent.h"

void UEnemyHPWidget::BindHp(class UMyStatComponent* StatComp)
{
	CurrentStatComp = StatComp;
	CurrentStatComp->OnHpChanged.AddUObject(this, &UEnemyHPWidget::UpdateHp);
	// 적의 HP 변화시 ProgressBar에 업데이트 해주어야 함.
}

void UEnemyHPWidget::UpdateHp()
{
	if (CurrentStatComp.IsValid())
		PB_HpBar->SetPercent(CurrentStatComp->GetEnemyHpRatio());
}

void UEnemyHPWidget::BindLevel(int32 Level)
{
	const FString MyLevel = FString::Printf(TEXT("%d"), Level);
	LevelText->SetText(FText::FromString(MyLevel));
	// 적의 레벨도 표시하기
}
