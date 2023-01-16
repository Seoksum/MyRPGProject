// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/GameModeBase.h"
#include "../MyRPGProjectGameModeBase.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MYRPGPROJECT_API AMyGameModeBase : public AMyRPGProjectGameModeBase
{
	GENERATED_BODY()
	

public:
	AMyGameModeBase();

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void PawnKilled(APawn* PawnKilled) override;


private:
	void EndGame(bool bIsPlayerWinner);


public:
	UPROPERTY(VisibleAnywhere)
	class UMyGameInstance* MyGameInstanceRef;

	UPROPERTY()
	TSubclassOf<UUserWidget> HUD_Class;

	UPROPERTY()
	UUserWidget* CurrentWidget;

	UPROPERTY(VisibleAnywhere)
	int32 CharTypeIndex;

	UPROPERTY(VisibleAnywhere)
	int32 CharMeshIndex;


};
