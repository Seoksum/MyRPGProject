// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Character_Parent.h"
#include "Interface/SkillAttackInterface.h"
#include "Character_Countess.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);

UCLASS()
class MYRPGPROJECT_API ACharacter_Countess : public ACharacter_Parent, public ISkillAttackInterface
{
	GENERATED_BODY()
	

public:

	ACharacter_Countess();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;



protected:

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	virtual void Attack() override;
	virtual void AttackQ_Implementation() override;
	virtual void AttackE_Implementation() override;
	virtual void AttackR_Implementation() override;
	

	UFUNCTION()
	void AttackCheck(int32 damage, float TraceDistance);

	virtual void PressClimbingUp() override;


private:

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);


private:

	UPROPERTY(VisibleAnywhere)
	class UAnimInstance_Countess* AnimInstance;

	UPROPERTY()
	int32 AttackIndex = 0;

	FOnAttackEnd OnAttackEnd;

	/*UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon_Gun> GunClass_L;*/

	UPROPERTY()
	class AWeapon_Sword* Sword_L;
};
