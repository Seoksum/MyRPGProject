// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Characters/Character_Parent.h"
#include "Components/SphereComponent.h"

AItem::AItem()
{
	ItemName = FText::FromString("Item");
	UseActionText = FText::FromString("Use");

}

void AItem::BeginPlay()
{
	Super::BeginPlay();

}

void AItem::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter_Parent* Player = Cast<ACharacter_Parent>(OtherActor);
	if (Player)
	{
		Player->CheckForInteractable(this);
		// 플레이어가 아이템과 Overlap되어있는 상태라면 this 아이템을 넘겨줍니다. 
	}
}

void AItem::Use(class ACharacter_Parent* PlayerCharacter)
{
}

void AItem::OnPickedUp(class ACharacter_Parent* PlayerCharacter)
{
	// 플레이어가 아이템을 수집했다면
	if (PlayerCharacter && PlayerCharacter->AddItemToInventory(this))
	{
		MeshComp->SetVisibility(false);
		MeshComp->SetSimulatePhysics(false);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 더 이상 시야에서 보이지 않고 충돌을 제거해줍니다. 
	}
}

