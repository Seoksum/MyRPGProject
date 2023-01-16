// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WinnerWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPGPROJECT_API UWinnerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeOnInitialized();

	UFUNCTION()
	void RestartGame();

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* RestartButton;

};
