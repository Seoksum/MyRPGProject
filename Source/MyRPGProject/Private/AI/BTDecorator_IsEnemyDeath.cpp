// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_IsEnemyDeath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Enemies/Enemy.h"
#include "AIController.h"


UBTDecorator_IsEnemyDeath::UBTDecorator_IsEnemyDeath()
{
	NodeName = TEXT("IsEnemyDeath");
}

bool UBTDecorator_IsEnemyDeath::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return false;

	auto Enemy = Cast<AEnemy>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Enemy"))));
	//auto Enemy = Cast<AEnemy_BuffRed>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Enemy"))));
	

	if (Enemy != nullptr)
		return bResult && Enemy->IsDeath;


	return false;
}