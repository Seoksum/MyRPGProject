// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CharacterSelectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYRPGPROJECT_API UCharacterSelectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual void NativeOnInitialized();

	UFUNCTION()
	virtual void GreystonePressed();

	UFUNCTION()
	virtual void CountessPressed();

	UFUNCTION()
	virtual void SelectPressed();

	UFUNCTION()
	virtual void NextButtonPressed();

	UFUNCTION()
	virtual void BeforeButtonPressed();


private:
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Greystone;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Countess;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_SelectCharacter;

	UPROPERTY(meta = (BindWidget))
	class UButton* NextBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* BeforeBtn;

	UPROPERTY(VisibleAnywhere)
	class UMyGameInstance* MyGameInstanceRef;

public:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ACharacterSelection> CurrentCharSelect;

	UPROPERTY(EditDefaultsOnly)
	class ACharacterSelection* SelectorActor;

	UPROPERTY(EditDefaultsOnly)
	int32 NowCharacterType;

};
