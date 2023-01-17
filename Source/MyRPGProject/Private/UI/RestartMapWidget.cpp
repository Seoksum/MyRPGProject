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
		RemainingText->SetText(FText::FromString(FStringRemaingCount)); // ���� �����ִ� ��� Ƚ���� �����ݴϴ�. 
	}
}

void URestartMapWidget::ClickRespawnBtn()
{
	// Respawn ��ư�� ������ ��
	MyPlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (MyPlayerController)
	{
		MyPlayerController->ChangeHUDState(MyPlayerController->HS_Ingame); // �ΰ��� â�� �߰� 
		MyPlayerController->RestartLevel(); // ���� ����� �մϴ�. 
	}
}

void URestartMapWidget::ClickTitleBtn()
{
	// Ÿ��Ʋ ��ư�� ������ ��
	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstanceRef)
	{
		MyGameInstanceRef->SetRemainingCount(3); // Respawn ���� Ƚ���� �ٽ� 3ȸ�� �ʱ�ȭ�մϴ�. 
		MyGameInstanceRef->IsRestarting = true;
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName("CharSelectMap")); // ĳ���� ���� ������ �̵��մϴ�. 
}

