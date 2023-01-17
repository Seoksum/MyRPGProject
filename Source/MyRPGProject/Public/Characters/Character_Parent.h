// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HealthInterface.h"
#include "Character_Parent.generated.h"




UCLASS()
class MYRPGPROJECT_API ACharacter_Parent : public ACharacter, public IHealthInterface
{
	GENERATED_BODY()

public:

	ACharacter_Parent();

protected:

	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	void UpDown(float Value);
	void LeftRight(float Value);
	void Yaw(float Value);
	void Pitch(float Value);

	void Sprint();
	void StopSprinting();

	void CameraZoom(const float Value);
	void BeginZoom();
	void EndZoom();

	void BeginCrouch();
	void EndCrouch();

	UFUNCTION()
		void DoubleJump();

	virtual void Landed(const FHitResult& Hit) override;

	virtual void Attack();

	void EndAttack_Q();
	void EndAttack_E();
	void EndAttack_R();

	void StopFire();

	virtual void PressClimbingUp(); // 1번 키를 눌러 캐릭터 벽 타기
	void ReleaseClimbingUp();

	virtual void OnDeath_Implementation() override;

	void IncreaseExp(int32 Exp);

public:

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Interact();

	//UFUNCTION(BlueprintPure, Category = "Inventory Functions")
	bool AddItemToInventory(class AItem* Item);

	//라인트레이스를 이용해서 플레이어 앞에 있는 액터가 interactable가능한 아이템인지 확인해주는 함수
	void CheckForInteractable(class AItem* PickItem);

	void UseItem(class AItem* Item);

	void SelectWeapon();
	void SelectInventory();

	virtual void SwitchWeapon(int32 Index);

	virtual void CameraShakeCheck();
	void CameraShake(TSubclassOf<class UCameraShakeBase> Cam);

	void LevelUp();

	class UMyStatComponent* GetMyStatComponent();

	class UInventoryComponent* GetInventoryComponent();

public:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		class UMyGameInstance* MyGameInstanceRef;

	UPROPERTY(VisibleAnywhere)
		class AMyPlayerController* MyPlayerController;

	//UPROPERTY(VisibleAnywhere)
	//class UMyStatComponent* Stat;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	//class UInventoryComponent* Inventory;

	class AItem* CurrentInteractable;

	UPROPERTY()
		bool bIsSprinting;

	UPROPERTY()
		int32 DoubleJumpCounter;

	UPROPERTY()
		bool IsDoubleJumping;

	UPROPERTY()
		int32 CurrentWeaponIndex;

	UPROPERTY(VisibleAnywhere, Category = "Climbing")
		bool bIsClimbingUp = false;

	UPROPERTY(VisibleAnywhere, Category = "Climbing")
		bool bIsOnWall = false;

	UPROPERTY(VisibleAnywhere, Category = "Climbing")
		bool bIsClimbingComplete = false;

	UPROPERTY(VisibleAnywhere)
		bool IsDeath;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class AEnemy> EnemyClass;

	UPROPERTY(EditDefaultsOnly)
		bool IsRestarting;



protected:

	UPROPERTY(VisibleAnywhere, Category = "Skill")
		bool IsAttacking = false;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
		bool IsAttackingQ = false;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
		bool IsAttackingE = false;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
		bool IsAttackingR = false;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
		int32 Remaining_SkillQ;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
		int32 Remaining_SkillE;

	UPROPERTY(VisibleAnywhere, Category = "Skill")
		int32 Remaining_SkillR;

	FTimerHandle QSkillHandle;
	FTimerHandle ESkillHandle;
	FTimerHandle RSkillHandle;

	FTimerHandle ClimbingHandle;

	UPROPERTY()
		int32 Level;

	UPROPERTY()
		float Mana;

	UPROPERTY()
		class AWeapon* CurrentWeapon;

	UPROPERTY(VisibleAnywhere)
		class UWidgetComponent* HpBar;

	UPROPERTY(VisibleAnywhere,Category="Stat")
	class UMyStatComponent* Stat;

	UPROPERTY(VisibleAnywhere, Category = "Inventory")
	class UInventoryComponent* Inventory;

	bool bWantsToZoom;

	UPROPERTY(EditDefaultsOnly, Category = "Player")
		float ZoomedFOV;

	// 보간 속도를 0.1 ~ 100 사이 값
	UPROPERTY(EditDefaultsOnly, Category = "Player", meta = (ClampMin = 0.1f, ClampMax = 100.f))
		float ZoomInterpSpeed;

	float DefaultFOV;

	UPROPERTY()
	class AWeapon_Sword* Sword;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AWeapon_Sword> SwordClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon_Gun> GunClass;

	UPROPERTY()
		class AWeapon_Gun* Gun;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon_Bow> BowClass;

	UPROPERTY()
		class AWeapon_Bow* Bow;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
		TSubclassOf<UCameraShakeBase> AttackCamShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UParticleSystem* LevelupEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USoundBase* LevelupSound;

	UPROPERTY(VisibleAnywhere)
		float LandYsize;


};
