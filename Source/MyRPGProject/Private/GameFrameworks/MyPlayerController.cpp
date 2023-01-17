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
	// 인게임 UI, 플레이중이라면 항상 떠있어야 하는 위젯입니다. 
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

	// 인벤토리 위젯 
	static ConstructorHelpers::FClassFinder<UInventoryWidget> Inventory_Class(TEXT("WidgetBlueprint'/Game/UI/WBP_Inventory.WBP_Inventory_C'"));
	if (Inventory_Class.Succeeded())
	{
		InventoryHUDClass = Inventory_Class.Class;
		InventoryWidget = CreateWidget(GetWorld(), InventoryHUDClass);
	}

	// 무기 선택 위젯
	static ConstructorHelpers::FClassFinder<UWeaponWidget> Weapon_Class(TEXT("WidgetBlueprint'/Game/UI/WBP_Weapon.WBP_Weapon_C'"));
	if (Weapon_Class.Succeeded())
		WeaponHUDClass = Weapon_Class.Class;

	// 게임 재시작 위젯
	static ConstructorHelpers::FClassFinder<UUserWidget> Restart_Class(TEXT("WidgetBlueprint'/Game/UI/WBP_Restart.WBP_Restart_C'"));
	if (Restart_Class.Succeeded())
		RestartHUDClass = Restart_Class.Class;
	
	// 십자선 위젯
	static ConstructorHelpers::FClassFinder<UUserWidget> Crosshair_Class(TEXT("WidgetBlueprint'/Game/UI/WBP_Crosshair.WBP_Crosshair_C'"));
	if (Crosshair_Class.Succeeded())
	{
		CrosshairClass = Crosshair_Class.Class;
		CrosshairWidget = CreateWidget(GetWorld(), CrosshairClass);
	}

	// 게임 승리 위젯
	static ConstructorHelpers::FClassFinder<UUserWidget> Winner_Class(TEXT("WidgetBlueprint'/Game/UI/WBP_Win.WBP_Win_C'"));
	if (Winner_Class.Succeeded())
		WinnerClass = Winner_Class.Class;

	HUDState = EHUDState::HS_Ingame;
}

void AMyPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	bOnInventoryHUD = false;	// 인벤토리 창이 떠 있으면 true, 아니면 false
	bOnWeaponHUD = false;		// 무기 창이 떠 있으면 true, 아니면 false
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
		// 플레이어가 승리라면 Winner 화면 띄우기
		WinnerWidget = CreateWidget<UWinnerWidget>(GetWorld(), WinnerClass);
		WinnerWidget->AddToViewport();
	}
	else
	{
		// 플레이어가 패배하면 Loser 화면 띄우기
		ChangeHUDState(HS_Restart);	
	}

	bShowMouseCursor = true;
	bEnableClickEvents = true;
	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	MyGameInstanceRef->SetNowLevel(2); 
	// 이기든 지든 레벨 2부터 시작합니다. 
}

void AMyPlayerController::RestartLevel()
{
	Super::RestartLevel();

	MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	int32 NowRemainingCount = MyGameInstanceRef->GetRemainingCount();
	NowRemainingCount--;
	MyGameInstanceRef->SetRemainingCount(NowRemainingCount);


	if (NowRemainingCount <= 0) // 더 이상 남은 목숨이 없다면 처음부터 시작
	{
		NowRemainingCount = 3;
		MyGameInstanceRef->SetRemainingCount(NowRemainingCount);
		UGameplayStatics::OpenLevel(GetWorld(), FName("CharSelectMap"));
	}

	else // 남은 목숨이 있다면 다른 HUD 보여주기
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
	// 적용하기전 위젯 체크
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

	if (!bOnInventoryHUD && !bOnWeaponHUD) // 어떠한 창도 없으면 마우스 커서를 숨깁니다. 
	{
		bShowMouseCursor = false;
		bEnableClickEvents = false;
	}
}

void AMyPlayerController::AddCrossWidget()
{
	// 화면에 십자선 창 추가하기 
	CrosshairWidget = CreateWidget(GetWorld(), CrosshairClass);
	if (CrosshairWidget)
		CrosshairWidget->AddToViewport();
}

void AMyPlayerController::RemoveCrossWidget()
{
	// 화면에 십자선 창 제거하기 
	if (CrosshairWidget)
		CrosshairWidget->RemoveFromViewport();
}