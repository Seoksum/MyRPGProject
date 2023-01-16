// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyRPGProjectGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MYRPGPROJECT_API AMyRPGProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	virtual void PawnKilled(APawn* PawnKilled);
	
};
