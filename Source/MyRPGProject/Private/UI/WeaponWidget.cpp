// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WeaponWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Character_Parent.h"
#include "GameFrameworks/MyPlayerController.h"
#include "GameFrameworks/MyGameInstance.h"


void UWeaponWidget::NativeOnInitialized()
{
	Button_Sword->OnClicked.AddDynamic(this, &UWeaponWidget::PressedSword);
	Button_Gun->OnClicked.AddDynamic(this, &UWeaponWidget::PressedGun);
	Button_Bow->OnClicked.AddDynamic(this, &UWeaponWidget::PressedBow);

}

void UWeaponWidget::PressedSword()
{
	ACharacter_Parent* Player = Cast<ACharacter_Parent>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AMyPlayerController* PC = Cast<AMyPlayerController>(Player->GetController());

	if (Player && PC)
	{
		if (Player->CurrentWeaponIndex == EWeapon::Sword)
			return;
		Player->SwitchWeapon(EWeapon::Sword);
		
		if (bZoomMode)
		{
			PC->RemoveCrossWidget();
			bZoomMode = false;
		}
	}
}

void UWeaponWidget::PressedGun()
{
	ACharacter_Parent* Player = Cast<ACharacter_Parent>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AMyPlayerController* PC = Cast<AMyPlayerController>(Player->GetController());

	if (Player && PC)
	{
		if (Player->CurrentWeaponIndex == EWeapon::Gun)
			return;
		Player->SwitchWeapon(EWeapon::Gun);

		if (!bZoomMode)
		{
			PC->AddCrossWidget();
			bZoomMode = true;
		}
	}
}

void UWeaponWidget::PressedBow()
{
	ACharacter_Parent* Player = Cast<ACharacter_Parent>(UGameplayStatics::GetPlayerCharacter(this, 0));
	AMyPlayerController* PC = Cast<AMyPlayerController>(Player->GetController());

	if (Player && PC)
	{
		if (Player->CurrentWeaponIndex == EWeapon::Bow)
			return;
		Player->SwitchWeapon(EWeapon::Bow);

		if (!bZoomMode)
		{
			PC->AddCrossWidget();
			bZoomMode = true;
		}
	}
}
