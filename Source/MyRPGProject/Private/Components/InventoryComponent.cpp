// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/InventoryComponent.h"
#include "Items/Item.h"
#include "GameFrameworks/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	Capacity = 20.f;


}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto& Item : DefaultItems)
	{
		AddItem(Item);
	}
}

void UInventoryComponent::AddItem(AItem* Item)
{
	if (Items.Num() >= Capacity || !Item)
		return;
	
	Item->OwingInventory = this;
	Items.Add(Item);
	// UI 업데이트
	OnInventoryUpdated.Broadcast();
}

void UInventoryComponent::RemoveItem(AItem* Item)
{
	if (Item)
	{
		Item->OwingInventory = nullptr;
		Items.RemoveSingle(Item);
		OnInventoryUpdated.Broadcast();
	}
	return;
}




