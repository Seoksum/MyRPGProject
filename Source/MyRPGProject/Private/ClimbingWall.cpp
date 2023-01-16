// Fill out your copyright notice in the Description page of Project Settings.


#include "ClimbingWall.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"


AClimbingWall::AClimbingWall()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/EnvironmentPack2/Meshes/SM_Plains_Wall_Straight_01.SM_Plains_Wall_Straight_01'"));
	if (SM.Succeeded())
	{
		MeshComp->SetStaticMesh(SM.Object);
	}
	RootComponent = MeshComp;

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComp"));
	CollisionComp->SetBoxExtent(FVector(10.f, 100.f, 100.f));
	CollisionComp->SetCollisionProfileName(FName("BlockAllDynamic"));
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
}

void AClimbingWall::BeginPlay()
{
	Super::BeginPlay();
	
}



