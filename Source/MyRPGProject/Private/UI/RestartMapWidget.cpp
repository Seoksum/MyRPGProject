// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RestartMapWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "GameFrameworks/MyGameInstance.h"
#include "GameFrameworks/MyPlayerController.h"
#include "Characters/Character_Greystone.h"


void URestartMapWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RespawnBtn->OnClicked.AddDynamic(this, &URestartMapWidget::ClickRespawnBtn);
	TitleBtn->OnClicked.AddDynamic(this, &URestartMapWidget::ClickTitleBtn);

	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstanceRef)
	{
		int32 NowRemainingCount = MyGameInstanceRef->GetRemainingCount();
		//NowRemainingCount--;
		const FString RMCount = FString::Printf(TEXT("%d"), NowRemainingCount);
		RemainingText->SetText(FText::FromString(RMCount));
	}
}

void URestartMapWidget::ClickRespawnBtn()
{
	//MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//if (MyGameInstanceRef)
	//{
	//	int32 NowRemainingCount = MyGameInstanceRef->GetRemainingCount();
	//	if (NowRemainingCount <= 0)
	//	{
	//		int32 NowRemainingCount = 3;
	//		MyGameInstanceRef->SetRemainingCount(NowRemainingCount);
	//		UGameplayStatics::OpenLevel(GetWorld(), FName("CharSelectMap"));
	//		return;
	//	}
	//}

	MyPlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (MyPlayerController)
	{
		MyPlayerController->ChangeHUDState(MyPlayerController->HS_Ingame);
		MyPlayerController->RestartLevel();
	}

}

void URestartMapWidget::ClickTitleBtn()
{
	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstanceRef)
	{
		MyGameInstanceRef->SetRemainingCount(3);
		MyGameInstanceRef->IsRestarting = true;
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName("CharSelectMap"));
}

