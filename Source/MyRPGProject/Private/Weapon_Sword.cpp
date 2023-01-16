// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Sword.h"
#include "Components/SkeletalMeshComponent.h"
#include "Enemies/Enemy.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AWeapon_Sword::AWeapon_Sword()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/Weapon/Sword/Sword1/SK_Blade_HeroSword11.SK_Blade_HeroSword11'"));
	if (SM.Succeeded())
	{
		MeshComp->SetSkeletalMesh(SM.Object);
	}
	MeshComp->SetGenerateOverlapEvents(true);
	RootComponent = MeshComp;

	AActor* MyOwner = GetOwner();
	
}

// Called when the game starts or when spawned
void AWeapon_Sword::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon_Sword::Attack(int32 damage, float TraceDistance, UParticleSystem* Particle)
{
	Super::Attack(damage, TraceDistance, Particle);

	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		TArray<FHitResult> TraceHits;
		FVector TraceStart = GetActorLocation();
		FVector TraceEnd = TraceStart + (MyOwner->GetActorForwardVector() * TraceDistance); // 150.0f
		FCollisionShape SweepShape = FCollisionShape::MakeSphere(200.0f);

		bool bResult = GetWorld()->SweepMultiByChannel(TraceHits, TraceStart, TraceEnd, FQuat::Identity, ECC_GameTraceChannel3, SweepShape);
		if (bResult)
		{
			for (FHitResult& Hit : TraceHits)
			{
				AEnemy* Enemy = Cast<AEnemy>(Hit.Actor);
				if (Enemy && !Enemy->IsDeath)
				{
					FDamageEvent DamageEvent;
					Enemy->TakeDamage(damage, DamageEvent, MyOwner->GetInstigatorController(), this);
					if (Particle)
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, Enemy->GetTransform());
					}
				}
			}
		}
	}

}



