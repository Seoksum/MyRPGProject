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
	// 게임에서 승리하면 나타나는 UI창으로, Restart 버튼을 누르면 게임을 다시 할 수 있습니다. 
	UMyGameInstance* MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstanceRef)
	{
		MyGameInstanceRef->SetRemainingCount(3); // 목숨을 다시 3으로 설정
		MyGameInstanceRef->IsRestarting = true;
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName("CharSelectMap")); // 캐릭터 선택 맵으로 이동합니다. 
}
