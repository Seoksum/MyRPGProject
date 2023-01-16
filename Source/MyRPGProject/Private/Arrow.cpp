// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Enemies/Enemy.h"


// Sets default values
AArrow::AArrow()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MESH"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("StaticMesh'/Game/Weapon/Archer/SM_Sparrow_Arrow.SM_Sparrow_Arrow'"));
	if (SM.Succeeded())
		MeshComp->SetStaticMesh(SM.Object);
	MeshComp->SetRelativeRotation(FRotator(90.f, 0.f, 0.f));
	

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	CollisionComp->SetBoxExtent(FVector(60.f, 40.f, 20.f));
	CollisionComp->SetCollisionProfileName(FName("Arrow"));
	CollisionComp->SetSimulatePhysics(true);
	CollisionComp->SetGenerateOverlapEvents(true);
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnBeginOverlap);
	RootComponent = CollisionComp;
	MeshComp->SetupAttachment(CollisionComp);

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComp->UpdatedComponent = CollisionComp;
	ProjectileMovementComp->ProjectileGravityScale = 1.f;
	ProjectileMovementComp->InitialSpeed = 4000.f;
	ProjectileMovementComp->MaxSpeed = 4000.f;
	ProjectileMovementComp->bRotationFollowsVelocity = false;
	ProjectileMovementComp->bInitialVelocityInLocalSpace = true;
	
	SetActorLocation(FVector(0.f, 0.f, 30.f));

	MuzzleSocketName = "MuzzleSocket";
	InitialLifeSpan = 3.f;
	BaseDamage = 30.f;
}

// Called when the game starts or when spawned
void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
	if (DefaultEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(DefaultEffect, MeshComp, MuzzleSocketName);
	}
}

void AArrow::OnBeginOverlap(UPrimitiveComponent* OverlappedcComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	FDamageEvent DamageEvent;
	FTransform EffectTrasform(FRotator::ZeroRotator, OtherActor->GetActorLocation());

	if (Enemy)
	{
		Enemy->TakeDamage(BaseDamage, DamageEvent, Enemy->GetController(), this);
		CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (SkillEffect)
		{
			UGameplayStatics::SpawnEmitterAttached(SkillEffect, MeshComp);
		}
	}
}

void AArrow::SetArrowSpeed(float ArrowSpeedVal)
{
	ProjectileMovementComp->InitialSpeed = ArrowSpeedVal;
}

