// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CharacterSelectionWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CharacterSelection.h"
#include "GameFrameworks/MyGameModeBase.h"
#include "GameFrameworks/CharacterSelectController.h"
#include "GameFrameworks/MyGameInstance.h"
#include "GameFrameworks/MyPlayerController.h"


void UCharacterSelectionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SelectorActor = Cast<ACharacterSelection>(UGameplayStatics::GetActorOfClass(this, CurrentCharSelect));
	NowCharacterType = 0;
}

void UCharacterSelectionWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (GetWorld())
	{
		MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	}

	if (Btn_Greystone == nullptr || Btn_Countess == nullptr)
		return;

	Btn_Greystone->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::GreystonePressed);
	Btn_Countess->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::CountessPressed);

	Btn_SelectCharacter->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::SelectPressed);

	NextBtn->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::NextButtonPressed);
	BeforeBtn->OnClicked.AddDynamic(this, &UCharacterSelectionWidget::BeforeButtonPressed);
}

void UCharacterSelectionWidget::GreystonePressed()
{
	SelectorActor->SelectCharacter(ECharacterIndex::Greystone);
	NowCharacterType = ECharacterIndex::Greystone;
}

void UCharacterSelectionWidget::CountessPressed()
{
	SelectorActor->SelectCharacter(ECharacterIndex::Countess);
	NowCharacterType = ECharacterIndex::Countess;
}

void UCharacterSelectionWidget::SelectPressed()
{
	UWorld* World = GetWorld();
	if (World)
	{
		ACharacterSelectController* PlayerController =
			Cast<ACharacterSelectController>(World->GetFirstPlayerController());

		if (PlayerController)
		{
			PlayerController->HideCharacterMenu();
		}
		if (MyGameInstanceRef)
		{
			MyGameInstanceRef->SetCharacterTypeIndex(NowCharacterType); // 캐릭터 타입 선택 
			MyGameInstanceRef->SetCharacterMeshIndex(SelectorActor->CharacterMeshIdx); // 캐릭터 SkeletalMesh 선택  
		}
		UGameplayStatics::OpenLevel(GetWorld(), FName("ZeroMap"),true);
	}
}

void UCharacterSelectionWidget::NextButtonPressed()
{
	SelectorActor->NextOrBefore(NowCharacterType, true);
}

void UCharacterSelectionWidget::BeforeButtonPressed()
{
	SelectorActor->NextOrBefore(NowCharacterType, false);
}

