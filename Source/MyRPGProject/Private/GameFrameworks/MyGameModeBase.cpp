// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFrameworks/MyGameModeBase.h"
#include "Characters/Character_Parent.h"
#include "Characters/Character_Greystone.h"
#include "Characters/Character_Countess.h"
#include "GameFrameworks/MyGameInstance.h"
#include "UI/InGame.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "GameFrameworks/MyPlayerController.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "AI/AIController_Enemy.h"
#include "Components/MyStatComponent.h"


AMyGameModeBase::AMyGameModeBase()
{

	PlayerControllerClass = AMyPlayerController::StaticClass();

	UWorld* World = GetWorld();
	if (World)
	{
		MyGameInstanceRef = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(World));
		if (MyGameInstanceRef)
		{
			// GameInstance�� ���� ������ ĳ���� Ÿ�԰� Mesh �ε����� �����ɴϴ�. 
			CharTypeIndex = MyGameInstanceRef->GetCharacterTypeIndex();
			CharMeshIndex = MyGameInstanceRef->GetCharacterMeshIndex();

			// ĳ������ ������Ʈ ������ �ð������� ���� Ȯ���ϱ� ���� C++������� ����� �������Ʈ�� ��ӹ޵��� ��������ϴ�. 
			// ĳ���Ͱ� Greystone
			if (CharTypeIndex == ECharacterIndex::Greystone)
			{
				static ConstructorHelpers::FClassFinder<ACharacter_Greystone> BP_Greystone1(TEXT("Blueprint'/Game/MainCharacter/Greystone/BP_Greystone.BP_Greystone_C'"));
				static ConstructorHelpers::FClassFinder<ACharacter_Greystone> BP_Greystone2(TEXT("Blueprint'/Game/MainCharacter/Greystone/BP_Greystone2.BP_Greystone2_C'"));
				static ConstructorHelpers::FClassFinder<ACharacter_Greystone> BP_Greystone3(TEXT("Blueprint'/Game/MainCharacter/Greystone/BP_Greystone3.BP_Greystone3_C'"));

				if (BP_Greystone1.Succeeded() && CharMeshIndex == 0)
					DefaultPawnClass = BP_Greystone1.Class;

				else if (BP_Greystone2.Succeeded() && CharMeshIndex == 1)
					DefaultPawnClass = BP_Greystone2.Class;

				else if (BP_Greystone3.Succeeded() && CharMeshIndex == 2)
					DefaultPawnClass = BP_Greystone3.Class; 
			}

			// ĳ���Ͱ� Countess
			else if (CharTypeIndex == ECharacterIndex::Countess)
			{
				static ConstructorHelpers::FClassFinder<ACharacter_Countess> BP_Countess1(TEXT("Blueprint'/Game/MainCharacter/Countess/BP_Countess.BP_Countess_C'"));
				static ConstructorHelpers::FClassFinder<ACharacter_Countess> BP_Countess2(TEXT("Blueprint'/Game/MainCharacter/Countess/BP_Countess2.BP_Countess2_C'"));
				static ConstructorHelpers::FClassFinder<ACharacter_Countess> BP_Countess3(TEXT("Blueprint'/Game/MainCharacter/Countess/BP_Countess3.BP_Countess3_C'"));

				if (BP_Countess1.Succeeded() && CharMeshIndex == 0)
					DefaultPawnClass = BP_Countess1.Class;

				else if (BP_Countess2.Succeeded() && CharMeshIndex == 1)
					DefaultPawnClass = BP_Countess2.Class;

				else if (BP_Countess3.Succeeded() && CharMeshIndex == 2)
					DefaultPawnClass = BP_Countess3.Class;

			}
		}
	}
}

void AMyGameModeBase::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

}

void AMyGameModeBase::PawnKilled(APawn* PawnKilled)
{
	Super::PawnKilled(PawnKilled);

	AMyPlayerController* PlayerController = Cast<AMyPlayerController>(PawnKilled->GetController());
	if (PlayerController)
	{
		EndGame(false); // Player�� ���� ���
	}

	for (AAIController_Enemy* Controller : TActorRange<AAIController_Enemy>(GetWorld()))
	{
		if (!Controller->IsDead())
		{		
			return; // ���� �������� �׾��ִ� AI�� ���ٸ� ���� return 
		}
	}
	EndGame(true); // ���� Ȯ�� �� �� �׾��ִٸ� Player �¸�
}

void AMyGameModeBase::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
		// �ش� Controller�� �����ϴ� Pawn�� ���� ���� �����ϱ� 
	}
}
