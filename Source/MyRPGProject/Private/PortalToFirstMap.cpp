// Fill out your copyright notice in the Description page of Project Settings.


#include "PortalToFirstMap.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/Character_Parent.h"
#include "GameFrameworks/MyGameInstance.h"


APortalToFirstMap::APortalToFirstMap()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> PM(TEXT("StaticMesh'/Game/Blueprints/Items/SM_TargettingPortal.SM_TargettingPortal'"));
	if (PM.Succeeded())
		MeshComp->SetStaticMesh(PM.Object);
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComp"));
	SphereComp->SetupAttachment(MeshComp);
	SphereComp->SetSphereRadius(100.f);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &APortalToFirstMap::OnBeginOverlap);
	SphereComp->OnComponentEndOverlap.AddDynamic(this, &APortalToFirstMap::OverlapEnds);

}

void APortalToFirstMap::OnBeginOverlap(UPrimitiveComponent* OverlappedcComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FLatentActionInfo LatentInfo;
	TArray<AActor*> PlayerStarts;
	FVector PlayerStartLocation;
	ACharacter_Parent* Player = Cast<ACharacter_Parent>(OtherActor);
	if (Player == nullptr)
		return;


	if (PortalTo == "FirstMap")
	{
		UGameplayStatics::LoadStreamLevel(GetWorld(), FName("FirstMap"), true, true, LatentInfo);
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("PlayerStart_FirstMap"), PlayerStarts);
	}

	else if (PortalTo == "BossMap")
	{
		UGameplayStatics::LoadStreamLevel(GetWorld(), FName("BossMap"), true, true, LatentInfo);
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("PS_BossMap"), PlayerStarts);
	}

	for (auto PlayerStart : PlayerStarts)
	{
		PlayerStartLocation = PlayerStart->GetActorLocation();
		Player->SetActorLocation(PlayerStartLocation);
	}

	return;
}

void APortalToFirstMap::OverlapEnds(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}



