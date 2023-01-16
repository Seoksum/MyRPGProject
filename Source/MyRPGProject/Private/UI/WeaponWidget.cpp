// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WeaponWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Character_Parent.h"
#include "GameFrameworks/MyPlayerController.h"
#include "GameFrameworks/MyGameInstance.h"


void UWeaponWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	Player = Cast<ACharacter_Parent>(GetOwningPlayerPawn());
	PC = Cast<AMyPlayerController>(Player->GetController());

	Button_Sword->OnClicked.AddDynamic(this, &UWeaponWidget::PressedSword);
	Button_Gun->OnClicked.AddDynamic(this, &UWeaponWidget::PressedGun);
	Button_Bow->OnClicked.AddDynamic(this, &UWeaponWidget::PressedBow);
}

void UWeaponWidget::PressedSword()
{
	if (Player && PC)
	{
		if (Player->CurrentWeaponIndex == EWeapon::Sword)
			return;
		Player->SwitchWeapon(EWeapon::Sword);

		if (bIsNowCrossHairMode)
		{
			PC->RemoveCrossWidget(); // 십자선이 있다면 제거하기 
			bIsNowCrossHairMode = false;
		}
	}
}

void UWeaponWidget::PressedGun()
{
	if (Player && PC)
	{
		if (Player->CurrentWeaponIndex == EWeapon::Gun)
			return;
		Player->SwitchWeapon(EWeapon::Gun);

		if (!bIsNowCrossHairMode)
		{
			PC->AddCrossWidget(); // 십자선이 없다면 추가하기 
			bIsNowCrossHairMode = true;
		}
	}
}

void UWeaponWidget::PressedBow()
{
	if (Player && PC)
	{
		if (Player->CurrentWeaponIndex == EWeapon::Bow)
			return;
		Player->SwitchWeapon(EWeapon::Bow);

		if (!bIsNowCrossHairMode)
		{
			PC->AddCrossWidget(); // 십자선이 없다면 추가하기 
			bIsNowCrossHairMode = true;
		}
	}
}
