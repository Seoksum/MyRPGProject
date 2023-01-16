// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameworks/CharacterSelectController.h"
#include "UI/CharacterSelectionWidget.h"
#include "Kismet/GameplayStatics.h"


ACharacterSelectController::ACharacterSelectController()
{

}

void ACharacterSelectController::BeginPlay()
{
	Super::BeginPlay();

}

void ACharacterSelectController::CreateCharacterMenu()
{
	if (BP_SelectWidget != nullptr)
	{
		SetInputMode(FInputModeUIOnly());
		bShowMouseCursor = true;

		SelectWidget = CreateWidget<UCharacterSelectionWidget>(this, BP_SelectWidget);
		SelectWidget->AddToViewport();
	}
}

void ACharacterSelectController::HideCharacterMenu()
{
	if (BP_SelectWidget != nullptr)
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;

		SelectWidget->RemoveFromParent();
		SelectWidget->Destruct();
	}
}


