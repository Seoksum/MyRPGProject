// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Weapon_Bow.generated.h"

/**
 * 
 */
UCLASS()
class MYRPGPROJECT_API AWeapon_Bow : public AWeapon
{
	GENERATED_BODY()

public:

	AWeapon_Bow();

	void ShootArrow();

protected:

	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AArrow> ArrowClass;

	UPROPERTY(EditDefaultsOnly)
	class AArrow* Arrow;

	UPROPERTY(EditDefaultsOnly)
	float ArrowSpeedVal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundBase* ShootSound;


};
