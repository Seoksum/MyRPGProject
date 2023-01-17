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
		const FString FStringRemaingCount = FString::Printf(TEXT("%d"), --NowRemainingCount);
		RemainingText->SetText(FText::FromString(FStringRemaingCount)); // 현재 남아있는 목숨 횟수를 보여줍니다. 
	}
}

void URestartMapWidget::ClickRespawnBtn()
{
	// Respawn 버튼을 눌렀을 때
	MyPlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (MyPlayerController)
	{
		MyPlayerController->ChangeHUDState(MyPlayerController->HS_Ingame); // 인게임 창이 뜨고 
		MyPlayerController->RestartLevel(); // 맵을 재시작 합니다. 
	}
}

void URestartMapWidget::ClickTitleBtn()
{
	// 타이틀 버튼을 눌렀을 때
	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstanceRef)
	{
		MyGameInstanceRef->SetRemainingCount(3); // Respawn 가능 횟수를 다시 3회로 초기화합니다. 
		MyGameInstanceRef->IsRestarting = true;
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName("CharSelectMap")); // 캐릭터 고르는 맵으로 이동합니다. 
}

