// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_SearchTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Character_Parent.h"
#include "Characters/Character_Greystone.h"
#include "AI/AIController_Enemy.h"

UBTService_SearchTarget::UBTService_SearchTarget()
{
	NodeName = TEXT("SearchTarget");
	Interval = 1.f;
}

void UBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto CurrentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (CurrentPawn == nullptr)
		return;

	UWorld* World = CurrentPawn->GetWorld();
	FVector Center = CurrentPawn->GetActorLocation();
	float SearchRadius = 1200.f;

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams(NAME_None, false, CurrentPawn);
	
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_Visibility,
		FCollisionShape::MakeSphere(SearchRadius),
		QueryParams);

	if (bResult)
	{
		for (auto& OverlapResult : OverlapResults)
		{
			ACharacter_Parent* Player = Cast<ACharacter_Parent>(OverlapResult.GetActor());

			if (Player && !Player->IsDeath)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), Player);
				return;
			}
			else
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
			}
		}

	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
	}
}
