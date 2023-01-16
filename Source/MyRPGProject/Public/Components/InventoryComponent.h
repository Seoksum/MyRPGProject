// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdated);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYRPGPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UInventoryComponent();

	virtual void BeginPlay() override;


public:
	void AddItem(class AItem* Item);
	void RemoveItem(class AItem* Item);

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	int32 Capacity;

	UPROPERTY(VisibleAnywhere, Category = "Items")
	TArray<class AItem*> Items;

	FOnInventoryUpdated OnInventoryUpdated;



	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<class AItem*> DefaultItems;

	UPROPERTY(VisibleAnywhere)
	class UMyGameInstance* MyGameInstanceRef;

	//UPROPERTY(BlueprintAssignable, Category = "Inventory")
};
