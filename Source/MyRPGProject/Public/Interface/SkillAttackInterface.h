// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SkillAttackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USkillAttackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYRPGPROJECT_API ISkillAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
		void AttackQ();
	virtual void AttackQ_Implementation() = 0;

	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
		void AttackE();
	virtual void AttackE_Implementation() = 0;

	UFUNCTION(BlueprintNativeEvent, Category = "Attack")
		void AttackR();
	virtual void AttackR_Implementation() = 0;
};
