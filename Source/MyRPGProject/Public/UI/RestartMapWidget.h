// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RestartMapWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPGPROJECT_API URestartMapWidget : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeOnInitialized();

	UFUNCTION()
	void ClickRespawnBtn();

	UFUNCTION()
	void ClickTitleBtn();


public:
	UPROPERTY(VisibleAnywhere)
	class UMyGameInstance* MyGameInstanceRef;

	UPROPERTY(VisibleAnywhere)
	class AMyPlayerController* MyPlayerController;

private:
	UPROPERTY(meta = (BindWIdget))
	class UButton* RespawnBtn;

	UPROPERTY(meta = (BindWIdget))
	class UButton* TitleBtn;

	UPROPERTY(meta = (BindWIdget))
	class UTextBlock* RemainingText;

};
