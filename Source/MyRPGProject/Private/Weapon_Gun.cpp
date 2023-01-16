// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "TimerManager.h"
#include "Characters/Character_Parent.h"
#include "Enemies/Enemy.h"


// Sets default values
AWeapon_Gun::AWeapon_Gun()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComp;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("SkeletalMesh'/Game/Weapon/Gun/WeaponMeshes/Weapons/Launcher.Launcher'"));
	if (SM.Succeeded())
	{
		MeshComp->SetSkeletalMesh(SM.Object);
	}
	SetActorScale3D(FVector(1.5f, 1.5f, 1.5f));

	MuzzleSocketName = "MuzzleFlashSocket";
	TracerTargetName = "Target";

	BaseDamage = 20.f;
	RateOfFire = 600.f; // 분 당 60개 총알
}

// Called when the game starts or when spawned
void AWeapon_Gun::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = 60.f / RateOfFire;
}

void AWeapon_Gun::Fire()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		FHitResult Hit;
		FVector EyeLocation;
		FRotator EyeRotation;
		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation); // OUT parameter

		FVector TraceStart = MeshComp->GetSocketLocation(MuzzleSocketName);
		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = TraceStart + (ShotDirection * 3000); // 종료 지점
		FVector TraceEndPoint = TraceEnd;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		QueryParams.AddIgnoredActor(MyOwner);

		bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_GameTraceChannel3, QueryParams);
		if (bHit)
		{
			AEnemy* Enemy = Cast<AEnemy>(Hit.GetActor());
			if (Enemy)
			{
				FDamageEvent DamageEvent;
				Enemy->TakeDamage(BaseDamage, DamageEvent, MyOwner->GetInstigatorController(), this);
				PlayImpactEffects(Hit.ImpactPoint, BloodImpactEffect);
			}
			else
			{
				PlayImpactEffects(Hit.ImpactPoint, DefaultImpactEffect);
			}

			TraceEndPoint = Hit.ImpactPoint;
		}

		PlayFireEffects(TraceEndPoint);
		LastFiredTime = GetWorld()->TimeSeconds; // 마지막 발사 시간을 기록(World의 time)
	}
}	// 1 + 0.1 - 1

void AWeapon_Gun::StartFire()
{
	float FirstDelay = FMath::Max(LastFiredTime + TimeBetweenShots - GetWorld()->TimeSeconds, 0.f);
	UE_LOG(LogTemp, Log, TEXT("First Delay : %f ,LastFiredTime : %f , TimeBetweenShots : %f, GetWolrd()->TimeSecons : %f"), FirstDelay, LastFiredTime, TimeBetweenShots,GetWorld()->TimeSeconds);
	GetWorldTimerManager().SetTimer(TimerHandle_TimeBetweenShots, this, &AWeapon_Gun::Fire, TimeBetweenShots, true, FirstDelay);
	UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
}

void AWeapon_Gun::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_TimeBetweenShots);
}

void AWeapon_Gun::PlayFireEffects(FVector TraceEndPoint)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, MuzzleSocketName);
	}

	if (TracerEffect)
	{
		// TracerEffect는 'P_SmokeTrail'인데 이 파티클시스템을 적용하기 위해서는 Source와 Target 부분이 있다. 
		FVector MuzzleLocation = MeshComp->GetSocketLocation(MuzzleSocketName);
		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);
		if (TracerComp)
		{
			TracerComp->SetVectorParameter("Target", TraceEndPoint); // P_SmokeTrail의 Target에서 Target Name을 찾아 인자로 넣는다.
		}
	}

	ACharacter_Parent* Player = Cast<ACharacter_Parent>(GetOwner());
	if (Player)
	{
		Player->CameraShake(FireCamShake);
	}
}

void AWeapon_Gun::PlayImpactEffects(FVector ImpactPoint, UParticleSystem* Particle)
{
	if (Particle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle, ImpactPoint);
	}
}
