// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CharacterSelection.generated.h"

UCLASS()
class MYRPGPROJECT_API ACharacterSelection : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACharacterSelection();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	void SelectCharacter(int32 Index);

	void NextOrBefore(int32 CharType, bool IsNext);

	void HideGreystone();
	void HideCountess();

	UPROPERTY(VisibleAnywhere)
	int32 CharacterMeshIdx = 0;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Greystones;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Countesses;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Greystone1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Greystone2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Greystone3;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Countess1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Countess2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Countess3;


	UPROPERTY(VisibleAnywhere)
	TArray<USkeletalMeshComponent*> GreyStoneArray;

	UPROPERTY(VisibleAnywhere)
	TArray<USkeletalMeshComponent*> CountessArray;


};
