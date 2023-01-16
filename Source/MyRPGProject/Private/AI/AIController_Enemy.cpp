// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIController_Enemy.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/Enemy.h"
#include "Characters/Character_Parent.h"
#include "GameFrameworks/MyGameInstance.h"


const FName AAIController_Enemy::HomePosKey(TEXT("HomePos"));
const FName AAIController_Enemy::PatrolPosKey(TEXT("PatrolPos"));

AAIController_Enemy::AAIController_Enemy()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT(TEXT("BehaviorTree'/Game/AI/BT_Enemy.BT_Enemy'"));
	if (BT.Succeeded())
	{
		BehaviorTree = BT.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBlackboardData> BD(TEXT("BlackboardData'/Game/AI/BB_Enemy.BB_Enemy'"));
	if (BD.Succeeded())
	{
		BlackboardData = BD.Object;
	}
}

void AAIController_Enemy::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (UseBlackboard(BlackboardData, Blackboard))
	{
		//AEnemy_BuffRed* Enemy = Cast<AEnemy_BuffRed>(InPawn);
		AEnemy* Enemy = Cast<AEnemy>(InPawn);

		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		Blackboard->SetValueAsObject(FName(TEXT("Enemy")), Enemy);

		if (RunBehaviorTree(BehaviorTree))
		{
			// TODO

		}
	}
}

void AAIController_Enemy::OnUnPossess()
{
	Super::OnUnPossess();

	//GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

bool AAIController_Enemy::IsDead() const
{
	AEnemy* ControlledEnemy = Cast<AEnemy>(GetPawn());
	if (ControlledEnemy)
	{
		bool bIsEnemyDeath = ControlledEnemy->EnemyIndex == EEnemy::SkeletonEnemy && ControlledEnemy->IsDeath;
		bool bIsBossDeath = ControlledEnemy->EnemyIndex == EEnemy::BossEnemy && ControlledEnemy->IsDeath;
		return bIsEnemyDeath && bIsBossDeath;
	}
	return true; // AI가 죽으면 폰이 AI 컨트롤러와 분리됨
}

bool AAIController_Enemy::IsBossDead() const
{
	AEnemy* ControlledCharacter = Cast<AEnemy>(GetPawn());
	if (ControlledCharacter)
	{
		return ControlledCharacter->EnemyIndex==EEnemy::BossEnemy && ControlledCharacter->IsDeath;
	}
	return true;
}

