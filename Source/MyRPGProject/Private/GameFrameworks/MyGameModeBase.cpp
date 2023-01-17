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
			// GameInstance를 통해 선택한 캐릭터 타입과 Mesh 인덱스를 가져옵니다. 
			CharTypeIndex = MyGameInstanceRef->GetCharacterTypeIndex();
			CharMeshIndex = MyGameInstanceRef->GetCharacterMeshIndex();

			// 캐릭터의 컴포넌트 설정을 시각적으로 보기 확인하기 위해 C++기반으로 만들고 블루프린트가 상속받도록 만들었습니다. 
			// 캐릭터가 Greystone
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

			// 캐릭터가 Countess
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
		EndGame(false); // Player가 졌을 경우
	}

	for (AAIController_Enemy* Controller : TActorRange<AAIController_Enemy>(GetWorld()))
	{
		if (!Controller->IsDead())
		{		
			return; // 현재 레벨에서 죽어있는 AI가 없다면 조기 return 
		}
	}
	EndGame(true); // 전부 확인 후 다 죽어있다면 Player 승리
}

void AMyGameModeBase::EndGame(bool bIsPlayerWinner)
{
	for (AController* Controller : TActorRange<AController>(GetWorld()))
	{
		bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
		Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
		// 해당 Controller를 소유하는 Pawn의 승패 여부 전달하기 
	}
}
