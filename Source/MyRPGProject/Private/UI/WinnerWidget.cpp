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
	// ���ӿ��� �¸��ϸ� ��Ÿ���� UIâ����, Restart ��ư�� ������ ������ �ٽ� �� �� �ֽ��ϴ�. 
	UMyGameInstance* MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (MyGameInstanceRef)
	{
		MyGameInstanceRef->SetRemainingCount(3); // ����� �ٽ� 3���� ����
		MyGameInstanceRef->IsRestarting = true;
	}
	UGameplayStatics::OpenLevel(GetWorld(), FName("CharSelectMap")); // ĳ���� ���� ������ �̵��մϴ�. 
}
