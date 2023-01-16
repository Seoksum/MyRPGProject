// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/Enemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/MyStatComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFrameworks/MyGameInstance.h"
#include "GameFrameworks/MyGameModeBase.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "UI/EnemyHPWidget.h"
#include "Characters/Character_Parent.h"
#include "Items/Item.h"
#include "AI/AIController_Enemy.h"
#include "Weapon_Sword.h"


// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -88.f), FRotator(0.f, -90.f, 0.f));
	GetCapsuleComponent()->BodyInstance.SetCollisionProfileName(FName("Enemy"));
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	// AI Controller »ý¼º
	AIControllerClass = AAIController_Enemy::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	Stat = CreateDefaultSubobject<UMyStatComponent>(TEXT("STAT"));
	
	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBAR"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 240.f));
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);

	static ConstructorHelpers::FClassFinder<UUserWidget> UW(TEXT("WidgetBlueprint'/Game/UI/WBP_EnemyHpBar.WBP_EnemyHpBar_C'"));
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
		HpBar->SetDrawSize(FVector2D(200.f, 50.f));
	}
	
	IsDeath = false;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	Stat->SetEnemyLevel(Level);
	Sword = GetWorld()->SpawnActor<AWeapon_Sword>(SwordClass);
	if (Sword)
	{
		Sword->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("SwordSocket"));
		Sword->SetOwner(this);
	}

}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (GetDistanceTo(Player) <= 2500.f)
		HpBar->SetVisibility(true);
	else
		HpBar->SetVisibility(false);
}

void AEnemy::Attack()
{
	
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ACharacter_Parent* Player = Cast<ACharacter_Parent>(DamageCauser);

	return DamageAmount;
}

void AEnemy::OnDeath_Implementation()
{
	if (IsDeath)
		return;

	IsDeath = true;
	AMyGameModeBase* GameMode = GetWorld()->GetAuthGameMode<AMyGameModeBase>();
	if (GameMode)
	{
		GameMode->PawnKilled(this);
	}
	
	HpBar->SetVisibility(false);
	OnEnemyDeath.Broadcast(EnemyExp);
	DetachFromControllerPendingDestroy();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UGameplayStatics::PlaySoundAtLocation(this, DeathSound, GetActorLocation());
	GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AEnemy::EnemyDeath, 2.f, false);
}


void AEnemy::EnemyDeath()
{
	FActorSpawnParameters spawnParams;
	FRotator rotator;
	FVector spawnLocation = GetActorLocation();

	if (PotionVal)
		GetWorld()->SpawnActor<AItem>(HpPotionToSpawn, spawnLocation, rotator, spawnParams);
	else
		GetWorld()->SpawnActor<AItem>(ManaPotionToSpawn, spawnLocation, rotator, spawnParams);

	GetWorldTimerManager().ClearTimer(DeathTimerHandle);
}



