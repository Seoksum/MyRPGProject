// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character_Parent.h"
#include "Interface/SkillAttackInterface.h"
#include "Character_Greystone.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);

UCLASS()
class MYRPGPROJECT_API ACharacter_Greystone : public ACharacter_Parent , public ISkillAttackInterface
{
	GENERATED_BODY()
	
public:

	ACharacter_Greystone();
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
	void SkillAttackCheck(int32 damage, float TraceDistance,class UParticleSystem* Particle);

	virtual void PressClimbingUp() override;

	virtual void CameraShakeCheck() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ParticleSystem", Meta = (AllowPrivateAccess = true))
		class UParticleSystem* AttackQ_Effect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ParticleSystem", Meta = (AllowPrivateAccess = true))
		class UParticleSystem* AttackE_Effect;


private:

	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	UPROPERTY(VisibleAnywhere)
	class UAnimInstance_Greystone* AnimInstance;

	UPROPERTY(VisibleAnywhere)
	int32 AttackIndex = 0;

	UPROPERTY(VisibleAnywhere)
	int32 TotalAttackIndex = 3;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
	bool AttackMoving;


};
