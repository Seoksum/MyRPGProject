// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CharacterSelectController.generated.h"

/**
 * 
 */
UCLASS()
class MYRPGPROJECT_API ACharacterSelectController : public APlayerController
{
	GENERATED_BODY()

public:
	ACharacterSelectController();

	virtual void BeginPlay() override;

	void CreateCharacterMenu();
	void HideCharacterMenu();


public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UCharacterSelectionWidget> BP_SelectWidget;

		class UCharacterSelectionWidget* SelectWidget;
	
};
