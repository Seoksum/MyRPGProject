// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class MYRPGPROJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:
	AItem();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void Use(class ACharacter_Parent* PlayerCharacter);
	// 아이템마다 사용 효과가 다를 것이므로 가상함수로 선언 

	void OnPickedUp(ACharacter_Parent* PlayerCharacter);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UWorld* World;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText UseActionText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	class USphereComponent* CollisionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
	class UTexture2D* Thumnail;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item")
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Item", meta = (MultiLine = true))
	FText ItemDescription;

	UPROPERTY()
	TWeakObjectPtr<class UInventoryComponent> OwingInventory;


};

