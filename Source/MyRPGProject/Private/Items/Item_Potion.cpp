// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item_Potion.h"
#include "Characters/Character_Parent.h"
#include "Components/MyStatComponent.h"
#include "Components/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/Image.h"
#include "Characters/Character_Parent.h"

AItem_Potion::AItem_Potion()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ItemName = FText::FromString("Enter an item name here...");

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	CollisionComp->InitSphereRadius(200.f);
	CollisionComp->SetupAttachment(MeshComp);
	CollisionComp->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

	HpPotionVal = 15.f;
	ManaPotionVal = 15.f;
}

void AItem_Potion::BeginPlay()
{
	Super::BeginPlay();

	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AItem_Potion::OnBeginOverlap);
}

void AItem_Potion::Use(ACharacter_Parent* PlayerCharacter)
{
	if (!PlayerCharacter)
		return;

	UMyStatComponent* Stat = PlayerCharacter->GetMyStatComponent();
	if (Stat)
	{
		if (ItemName.EqualTo(FText::FromString("HpPotion")))
		{
			Stat->UseHpPotion(HpPotionVal); // HpPotionVal = 15.f;
		}
		else if (ItemName.EqualTo(FText::FromString("ManaPotion")))
		{
			Stat->UseManaPotion(ManaPotionVal); // ManaPotionVal = 15.f;
		}
	}
}

