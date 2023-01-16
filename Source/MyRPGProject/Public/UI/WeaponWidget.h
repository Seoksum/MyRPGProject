// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPGPROJECT_API UWeaponWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized();

	UFUNCTION()
	virtual void PressedSword();

	UFUNCTION()
	virtual void PressedGun();

	UFUNCTION()
	virtual void PressedBow();

	bool bIsNowCrossHairMode; // Arrow or Gun 무기일 때 

	class ACharacter_Parent* Player;

	class AMyPlayerController* PC;


public:
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Sword;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Gun;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Bow;

};
