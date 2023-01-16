// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGame.generated.h"

/**
 * 
 */
UCLASS()
class MYRPGPROJECT_API UInGame : public UUserWidget
{
	GENERATED_BODY()
	

public:

	void SetGreystone();
	void SetCountess();
	void SetCharacterSkillThumbnail(FString Q_Path, FString E_Path, FString R_Path);

	void SetLevel();
	void BindLevel(class UMyStatComponent* StatComp);

	void BindHp(class UMyStatComponent* StatComp);
	void UpdateHp();

	void BindMana(class UMyStatComponent* StatComp);
	void UpdateMana();

public:

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Level;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Q;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_E;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_R;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Q;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_E;

	UPROPERTY(meta = (BindWidget))
	class UImage* Image_R;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HP;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_Mana;

	UPROPERTY(VisibleAnywhere)
	class UMyGameInstance* MyGameInstanceRef;

	TWeakObjectPtr<class UMyStatComponent> CurrentStatComp;


};
