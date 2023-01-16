// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "Enemies/Enemy.h"
#include "AI/AIController_Enemy.h"
#include "Characters/Character_Parent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"


UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;

}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	//auto Enemy = Cast<AEnemy_BuffRed>(OwnerComp.GetAIOwner()->GetPawn());
	auto Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	auto Player = Cast<ACharacter_Parent>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));

	if (Enemy == nullptr)
		return EBTNodeResult::Failed;

	if (Enemy->IsDeath)
		return EBTNodeResult::Failed;

	if (Player)
	{
		FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), Player->GetActorLocation());
		Enemy->SetActorRotation(newRotation);
	}

	Enemy->Attack();

	bIsAttacking = true;

	Enemy->OnAttackEnd.AddLambda([this]()
		{
			bIsAttacking = false;
		});


	return Result;

}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (bIsAttacking == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
