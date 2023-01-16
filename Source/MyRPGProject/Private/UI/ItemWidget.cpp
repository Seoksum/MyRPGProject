// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ItemWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/InventoryComponent.h"
#include "Characters/Character_Parent.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Item.h"


void UItemWidget::NativeConstruct()
{
	Super::NativeConstruct();

	
}

void UItemWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UseButton->OnClicked.AddDynamic(this, &UItemWidget::OnClickedUseButton); // ������ ��ư Ŭ��
	UseButton->ToolTipTextDelegate.BindUFunction(this, FName("GetToolTipUseButton")); // �����ۿ� ���콺�� ���ٴ�� ���� ǥ��
}

void UItemWidget::OnClickedUseButton()
{
	ACharacter_Parent* Player = Cast<ACharacter_Parent>(GetOwningPlayerPawn());
	if (Player)
	{
		Player->UseItem(Item); // �� �����ۿ� �´� ȿ���� �߻��մϴ�. 
	}
}

FText UItemWidget::GetToolTipUseButton()
{
	// �������� ����� �˷��ݴϴ�.
	return (("Click to {0} {1},{2}"), Item->UseActionText, Item->ItemName, Item->ItemDescription);
}

void UItemWidget::SetItemInfo(class AItem* PickItem)
{
	// �������� ����ϰ� �̸��� ǥ���մϴ�.  
	Item = PickItem;

	UTexture2D* MyThumbnail = Item->Thumnail;
	if (MyThumbnail)
		Thumbnail->SetBrushFromTexture(MyThumbnail);
	
	FText Name = Item->ItemName;
	ItemName->SetText(Name);
}
