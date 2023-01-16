// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Weapon_Sword.generated.h"

UCLASS()
class MYRPGPROJECT_API AWeapon_Sword : public AWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon_Sword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void Attack(int32 damage, float TraceDistance, class UParticleSystem* Particle) override;





};
