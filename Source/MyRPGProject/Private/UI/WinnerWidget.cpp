// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WinnerWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "GameFrameworks/MyGameInstance.h"


void UWinnerWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RestartButton->OnClicked.AddDynamic(this, &UWinnerWidget::RestartGame);
}

void UWinnerWidget::RestartGame()
{
	UMyGameInstance* MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstanceRef)
	{
		MyGameInstanceRef->SetRemainingCount(3);
		MyGameInstanceRef->IsRestarting = true;
		//MyGameInstanceRef->SetNowLevel(2);
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName("CharSelectMap"));
}
