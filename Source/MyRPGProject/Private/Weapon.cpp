// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::Attack()
{
}

void AWeapon::Attack(int32 damage, float TraceDistance, UParticleSystem* Particle)
{
}

