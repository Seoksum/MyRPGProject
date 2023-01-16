// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/AnimInstance_Countess.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Characters/Character_Countess.h"
#include "Components/MyStatComponent.h"


UAnimInstance_Countess::UAnimInstance_Countess()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AM(TEXT("AnimMontage'/Game/MainCharacter/Countess/Montages/Attack_Default.Attack_Default'"));
	if (AM.Succeeded())
	{
		AttackMontage = AM.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AQ(TEXT("AnimMontage'/Game/MainCharacter/Countess/Montages/Attack_Q.Attack_Q'"));
	if (AQ.Succeeded())
	{
		Attack_Q = AQ.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AE(TEXT("AnimMontage'/Game/MainCharacter/Countess/Montages/Attack_E.Attack_E'"));
	if (AE.Succeeded())
	{
		Attack_E = AE.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AR(TEXT("AnimMontage'/Game/MainCharacter/Countess/Montages/Attack_R.Attack_R'"));
	if (AR.Succeeded())
	{
		Attack_R = AR.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> CC(TEXT("AnimMontage'/Game/MainCharacter/Greystone/Montages/ClimbingComplete.ClimbingComplete'"));
	if (CC.Succeeded())
	{
		ClimbingCompleteMontage = CC.Object;
	}
}

void UAnimInstance_Countess::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner(); //나를 포함하고 있는 Pawn을 Get하겠다. 
	if (IsValid(Pawn))
	{
		Speed = Pawn->GetVelocity().Size();

		auto PlayerCharacter = Cast<ACharacter_Countess>(Pawn);
		if (PlayerCharacter)
		{
			Speed = PlayerCharacter->GetVelocity().Size();
			Direction = CalculateDirection(PlayerCharacter->GetVelocity(), PlayerCharacter->GetActorRotation());

			IsFalling = PlayerCharacter->GetMovementComponent()->IsFalling();
			IsDoubleJump = PlayerCharacter->IsDoubleJumping;

			WeaponIdx = PlayerCharacter->CurrentWeaponIndex;

			IsCrouched = PlayerCharacter->bIsCrouched;
			IsOnWall = PlayerCharacter->bIsOnWall;
			IsClimbingUp = PlayerCharacter->bIsClimbingUp;
		}
	}
}

void UAnimInstance_Countess::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.f);
}

void UAnimInstance_Countess::PlayAttackMontageQ()
{
	Montage_Play(Attack_Q, 1.f);
}

void UAnimInstance_Countess::PlayAttackMontageE()
{
	Montage_Play(Attack_E, 1.f);
}

void UAnimInstance_Countess::PlayAttackMontageR()
{
	Montage_Play(Attack_R, 1.f);
}

void UAnimInstance_Countess::JumpToSection(int32 SectionIndex)
{
	FName Name = GetAttackMontageName(SectionIndex);
	Montage_JumpToSection(Name, AttackMontage);
}

FName UAnimInstance_Countess::GetAttackMontageName(int32 SectionIndex)
{
	return FName(*FString::Printf(TEXT("Attack%d"), SectionIndex));
}

void UAnimInstance_Countess::PlayClimbingComplete()
{
	Montage_Play(ClimbingCompleteMontage, 1.f);
}

void UAnimInstance_Countess::AnimNotify_AttackHit()
{
	auto Player = Cast<ACharacter_Countess>(TryGetPawnOwner());
	if (Player)
	{
		auto Stat = Player->GetMyStatComponent();
		TraceDistance = 150.f;
		OnAttackHit.Broadcast(Stat->GetAttack(), TraceDistance);
	}
}

void UAnimInstance_Countess::AnimNotify_AttackHit_Q()
{
	auto Player = Cast<ACharacter_Countess>(TryGetPawnOwner());
	if (Player)
	{
		auto Stat = Player->GetMyStatComponent();
		TraceDistance = 200.f;
		OnAttackHit_Q.Broadcast(Stat->GetAttack_Q(), TraceDistance);
	}
}

void UAnimInstance_Countess::AnimNotify_AttackHit_E()
{
	auto Player = Cast<ACharacter_Countess>(TryGetPawnOwner());
	if (Player)
	{
		auto Stat = Player->GetMyStatComponent();
		TraceDistance = 300.f;
		OnAttackHit_E.Broadcast(Stat->GetAttack_E(), TraceDistance);
	}
}

void UAnimInstance_Countess::AnimNotify_AttackHit_R()
{
	auto Player = Cast<ACharacter_Countess>(TryGetPawnOwner());
	if (Player)
	{
		auto Stat = Player->GetMyStatComponent();
		TraceDistance = 400.f;
		OnAttackHit_R.Broadcast(Stat->GetAttack_R(), TraceDistance);
	}
}

