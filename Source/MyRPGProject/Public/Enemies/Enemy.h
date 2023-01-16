// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/HealthInterface.h"
#include "Enemy.generated.h"


//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEnemyDeath);
//DECLARE_MULTICAST_DELEGATE(FOnEnemyDeath);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyDeath, int32);
DECLARE_MULTICAST_DELEGATE(FOnAttackEnd);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBossJump, TSubclassOf<class UCameraShakeBase>);

UCLASS()
class MYRPGPROJECT_API AEnemy : public ACharacter, public IHealthInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	virtual void OnDeath_Implementation();
	void EnemyDeath();

	virtual void Attack();


protected:

	UPROPERTY(VisibleAnywhere,Category="Components")
	class UWidgetComponent* HpBar;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	class UEnemy_AnimInstance* AnimInstance;

	FTimerHandle DeathTimerHandle;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	int32 Level;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	int32 EnemyExp;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AItem> HpPotionToSpawn;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AItem> ManaPotionToSpawn;

public:

	UPROPERTY(VisibleAnywhere, Category = "Components")
	class UMyStatComponent* Stat;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	bool IsDeath = false;

	UPROPERTY(VisibleAnywhere, Category = "Enemy")
	bool IsAttacking = false;

	UPROPERTY(EditAnywhere, Category = "Enemy")
	bool PotionVal;

	UPROPERTY(EditDefaultsOnly, Category = "Enemy")
	TSubclassOf<class AWeapon_Sword> SwordClass;

	UPROPERTY()
	class AWeapon_Sword* Sword;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USoundBase* DeathSound;

	FOnEnemyDeath OnEnemyDeath;

	FOnAttackEnd OnAttackEnd;

	FOnBossJump OnBossJump;

	UPROPERTY()
	int32 EnemyIndex;

};
