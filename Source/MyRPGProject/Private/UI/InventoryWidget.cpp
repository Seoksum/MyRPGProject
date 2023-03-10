// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InventoryWidget.h"
#include "Characters/Character_Parent.h"
#include "Components/InventoryComponent.h"
#include "Components/Button.h"
#include "Components/WrapBox.h"
#include "UI/ItemWidget.h"
#include "Items/Item.h"
#include "Kismet/GameplayStatics.h"
#include "GameFrameworks/MyGameInstance.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ACharacter_Parent* Player = Cast<ACharacter_Parent>(GetOwningPlayerPawn()); 
	// UI와 연결된 Pawn을 가져옴 
	if (Player)
	{
		Inventory = Player->GetInventoryComponent();
		if (Inventory)
		{
			Inventory->OnInventoryUpdated.AddDynamic(this, &UInventoryWidget::RefreshInventory);
		}
	}
}

void UInventoryWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CloseButton->OnClicked.AddDynamic(this, &UInventoryWidget::OnClickedCloseButton);
}

void UInventoryWidget::RefreshInventory()
{
	InventoryBox->ClearChildren(); // 중복 위젯 생성을 방지하기 위해 우선 Clear 해줍니다. 
	TArray<class AItem*> CurrentItems = Inventory->GetItems();

	for (auto& Item : CurrentItems)
	{
		if (Item)
		{
			auto ItemWidget = CreateWidget<UItemWidget>(GetOwningPlayer(), ItemHUDClass);
			if (ItemWidget)
			{
				InventoryBox->AddChildToWrapBox(ItemWidget);
				ItemWidget->SetItemInfo(Item);
			}
		}
	}
}

void UInventoryWidget::OnClickedCloseButton()
{
	RemoveFromParent();
	GetOwningPlayer()->SetShowMouseCursor(false);
}
