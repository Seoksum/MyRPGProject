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
		HS_Ingame,		// �ΰ��� UI(Q,E,R ��ų)
		HS_Inventory,	// �κ��丮 UI
		HS_Weapon,		// ���� UI
		HS_Restart,		// ����� UI
	};
	void ApplyHUDChanges();	// ���ϴ� HUD�� ����(�ٲٱ�)�ϱ����� �Լ�
	uint8 GetHUDState();

	void ChangeHUDState(uint8 NewState); // ���ο� hudstate�� �޾ƿ� �� applyHUDChanges ȣ��

	bool ApplyHUD(TSubclassOf<class UUserWidget> WidgetToApply, bool bShowMouseCursor, bool EnableClickEvents); // screen�� hud�� ������ �� �������� ��ȯ

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

	bool bOnInventoryHUD;	// ��Ű�� ����� �κ��丮 â �ѱ�/���� �� �� ����
	bool bOnWeaponHUD;		// oŰ�� ����� ���� â �ѱ�/���� �� �� ����
};
