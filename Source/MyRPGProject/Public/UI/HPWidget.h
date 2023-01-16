// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HPWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPGPROJECT_API UHPWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindHp(class UMyStatComponent* StatComp);
	void UpdateHp();

	void BindMana(class UMyStatComponent* StatComp);
	void UpdateMana();

private:
	TWeakObjectPtr<class UMyStatComponent> CurrentStatComp;
	// 플레이어 StatComponent를 Weak포인터로 참조하기 

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HpBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_ManaBar;
	
};
