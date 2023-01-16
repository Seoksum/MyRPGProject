// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYRPGPROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	

public:
	AMyPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;

	virtual void GameHasEnded(AActor* EndGameFocus, bool bIsWinner) override;

	virtual void RestartLevel() override;

	enum EHUDState : uint8
	{
		HS_Ingame,		// 인게임 UI(Q,E,R 스킬)
		HS_Inventory,	// 인벤토리 UI
		HS_Weapon,		// 무기 UI
		HS_Restart,		// 재시작 UI
	};
	void ApplyHUDChanges();	// 원하는 HUD로 적용(바꾸기)하기위한 함수
	uint8 GetHUDState();

	void ChangeHUDState(uint8 NewState); // 새로운 hudstate를 받아온 후 applyHUDChanges 호출

	bool ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool bShowMouseCursor, bool EnableClickEvents); // screen에 hud를 적용한 후 성공여부 반환

	void RemoveHUD(uint8 NewState);

	void AddCrossWidget();
	void RemoveCrossWidget();

private:

	UPROPERTY()
	UUserWidget* InventoryWidget;

	UPROPERTY()
	UUserWidget* RestartWidget;

	UPROPERTY()
	UUserWidget* WeaponWidget;

	UPROPERTY()
	UUserWidget* CrosshairWidget;

	UPROPERTY()
	UUserWidget* WinnerWidget;


public:
	uint8 HUDState;

	UPROPERTY()
	UUserWidget* CurrentWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = true))
	TSubclassOf<UUserWidget> IngameHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = true))
	TSubclassOf<UUserWidget> InventoryHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = true))
	TSubclassOf<UUserWidget> RestartHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = true))
	TSubclassOf<UUserWidget> WeaponHUDClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = true))
	TSubclassOf<UUserWidget> CrosshairClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUDWidgets", Meta = (BlueprintProtected = true))
	TSubclassOf<UUserWidget> WinnerClass;


	UPROPERTY(VisibleAnywhere)
	class UMyGameInstance* MyGameInstanceRef;

	bool bOnInventoryHUD;	// 탭키를 사용해 인벤토리 창 켜기/끄기 둘 다 가능
	bool bOnWeaponHUD;		// o키를 사용해 무기 창 켜기/끄기 둘 다 가능
};
