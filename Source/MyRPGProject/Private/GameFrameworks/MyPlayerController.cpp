// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameworks/MyPlayerController.h"
#include "UI/InGame.h"
#include "UI/WeaponWidget.h"
#include "UI/RestartMapWidget.h"
#include "UI/WinnerWidget.h"
#include "Characters/Character_Greystone.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "GameFrameworks/MyGameInstance.h"
#include "UI/InventoryWidget.h"


AMyPlayerController::AMyPlayerController()
{
	// �ΰ��� UI, �÷������̶�� �׻� ���־�� �ϴ� �����Դϴ�. 
	static ConstructorHelpers::FClassFinder<UInGame> Ingame_Class(TEXT("WidgetBlueprint'/Game/UI/WBP_InGame.WBP_InGame_C'"));
	if (Ingame_Class.Succeeded())
	{
		IngameHUDClass = Ingame_Class.Class;
		CurrentWidget = CreateWidget(GetWorld(), IngameHUDClass);
		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}

	// �κ��丮 ���� 
	static ConstructorHelpers::FClassFinder<UInventoryWidget> Inventory_Class(TEXT("WidgetBlueprint'/Game/UI/WBP_Inventory.WBP_Inventory_C'"));
	if (Inventory_Class.Succeeded())
	{
		InventoryHUDClass = Inventory_Class.Class;
		InventoryWidget = CreateWidget(GetWorld(), InventoryHUDClass);
	}

	// ���� ���� ����
	static ConstructorHelpers::FClassFinder<UWeaponWidget> Weapon_Class(TEXT("WidgetBlueprint'/Game/UI/WBP_Weapon.WBP_Weapon_C'"));
	if (Weapon_Class.Succeeded())
		WeaponHUDClass = Weapon_Class.Class;

	// ���� ����� ����
	static ConstructorHelpers::FClassFinder<UUserWidget> Restart_Class(TEXT("WidgetBlueprint'/Game/UI/WBP_Restart.WBP_Restart_C'"));
	if (Restart_Class.Succeeded())
		RestartHUDClass = Restart_Class.Class;
	
	// ���ڼ� ����
	static ConstructorHelpers::FClassFinder<UUserWidget> Crosshair_Class(TEXT("WidgetBlueprint'/Game/UI/WBP_Crosshair.WBP_Crosshair_C'"));
	if (Crosshair_Class.Succeeded())
	{
		CrosshairClass = Crosshair_Class.Class;
		CrosshairWidget = CreateWidget(GetWorld(), CrosshairClass);
	}

	// ���� �¸� ����
	static ConstructorHelpers::FClassFinder<UUserWidget> Winner_Class(TEXT("WidgetBlueprint'/Game/UI/WBP_Win.WBP_Win_C'"));
	if (Winner_Class.Succeeded())
		WinnerClass = Winner_Class.Class;

	HUDState = EHUDState::HS_Ingame;
}

void AMyPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	bOnInventoryHUD = false;	// �κ��丮 â�� �� ������ true, �ƴϸ� false
	bOnWeaponHUD = false;		// ���� â�� �� ������ true, �ƴϸ� false
}

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	
}

void AMyPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	if (bIsWinner)
	{
		// �÷��̾ �¸���� Winner ȭ�� ����
		WinnerWidget = CreateWidget<UWinnerWidget>(GetWorld(), WinnerClass);
		WinnerWidget->AddToViewport();
	}
	else
	{
		// �÷��̾ �й��ϸ� Loser ȭ�� ����
		ChangeHUDState(HS_Restart);	
	}

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MyGameInstanceRef->SetNowLevel(2); 
	// �̱�� ���� ���� 2���� �����մϴ�. 
}

void AMyPlayerController::RestartLevel()
{
	Super::RestartLevel();

	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	int32 NowRemainingCount = MyGameInstanceRef->GetRemainingCount();
	NowRemainingCount--;
	MyGameInstanceRef->SetRemainingCount(NowRemainingCount);


	if (NowRemainingCount <= 0) // �� �̻� ���� ����� ���ٸ� ó������ ����
	{
		NowRemainingCount = 3;
		MyGameInstanceRef->SetRemainingCount(NowRemainingCount);
		UGameplayStatics::OpenLevel(GetWorld(), FName("CharSelectMap"));
	}

	else // ���� ����� �ִٸ� �ٸ� HUD �����ֱ�
	{
		ChangeHUDState(HS_Restart);
		MyGameInstanceRef->IsRestarting = true;
	}
}

void AMyPlayerController::ApplyHUDChanges()
{
	switch (HUDState)
	{
	case EHUDState::HS_Ingame:
	{
		ApplyHUD(IngameHUDClass, false, false);

		break;
	}

	case EHUDState::HS_Inventory:
	{
		ApplyHUD(InventoryHUDClass, true, true);
		break;
	}

	case EHUDState::HS_Weapon:
	{
		ApplyHUD(WeaponHUDClass, true, false);
		break;
	}

	case EHUDState::HS_Restart:
	{
		ApplyHUD(RestartHUDClass, true, true);
		break;
	}
	
	default:
	{
		ApplyHUD(IngameHUDClass, false, false);
		break;
	}
	}
}

uint8 AMyPlayerController::GetHUDState()
{
	return HUDState;
}

void AMyPlayerController::ChangeHUDState(uint8 NewState)
{
	HUDState = NewState;
	ApplyHUDChanges();
}

bool AMyPlayerController::ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool IsShowMouseCursor, bool IsEnableClickEvents)
{
	// �����ϱ��� ���� üũ
	if (WidgetToApply)
	{
		bShowMouseCursor = IsShowMouseCursor;
		bEnableClickEvents = IsEnableClickEvents;

		if (WidgetToApply == InventoryHUDClass)
		{
			InventoryWidget->AddToViewport();
			bOnInventoryHUD = true;
			return true;
		}

		else if (WidgetToApply == WeaponHUDClass)
		{
			WeaponWidget = CreateWidget<UWeaponWidget>(GetWorld(), WidgetToApply);
			WeaponWidget->AddToViewport();
			bOnWeaponHUD = true;
			return true;
		}

		else if (WidgetToApply == RestartHUDClass)
		{
			RestartWidget = CreateWidget<URestartMapWidget>(GetWorld(), WidgetToApply);
			CurrentWidget->RemoveFromViewport();
			RestartWidget->AddToViewport();
			return true;
		}


		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
			return true;
		}
		return false;
	}
	return false;
}

void AMyPlayerController::RemoveHUD(uint8 State)
{
	switch (State)
	{
	case EHUDState::HS_Inventory:
	{
		InventoryWidget->RemoveFromViewport();
		bOnInventoryHUD = false;
		break;
	}

	case EHUDState::HS_Weapon:
	{
		WeaponWidget->RemoveFromViewport();
		bOnWeaponHUD = false;
		break;
	}

	case EHUDState::HS_Restart:
	{
		RestartWidget->RemoveFromViewport();
		break;
	}

	default:
	{
		ApplyHUD(IngameHUDClass, false, false);
		break;
	}
	}

	if (!bOnInventoryHUD && !bOnWeaponHUD) // ��� â�� ������ ���콺 Ŀ���� ����ϴ�. 
	{
		bShowMouseCursor = false;
		bEnableClickEvents = false;
	}
}

void AMyPlayerController::AddCrossWidget()
{
	// ȭ�鿡 ���ڼ� â �߰��ϱ� 
	CrosshairWidget = CreateWidget(GetWorld(), CrosshairClass);
	if (CrosshairWidget)
		CrosshairWidget->AddToViewport();
}

void AMyPlayerController::RemoveCrossWidget()
{
	// ȭ�鿡 ���ڼ� â �����ϱ� 
	if (CrosshairWidget)
		CrosshairWidget->RemoveFromViewport();
}