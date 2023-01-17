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
		// �÷��̾ �����۰� Overlap�Ǿ��ִ� ���¶�� this �������� �Ѱ��ݴϴ�. 
	}
}

void AItem::Use(class ACharacter_Parent* PlayerCharacter)
{
}

void AItem::OnPickedUp(class ACharacter_Parent* PlayerCharacter)
{
	// �÷��̾ �������� �����ߴٸ�
	if (PlayerCharacter && PlayerCharacter->AddItemToInventory(this))
	{
		MeshComp->SetVisibility(false);
		MeshComp->SetSimulatePhysics(false);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// �� �̻� �þ߿��� ������ �ʰ� �浹�� �������ݴϴ�. 
	}
}

