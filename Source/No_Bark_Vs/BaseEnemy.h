// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseCharacter.h"
#include "BaseEnemy.generated.h"

/**
 * 
 */




UCLASS()
class NO_BARK_VS_API ABaseEnemy : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	ABaseEnemy();

	UPROPERTY(BlueprintReadWrite, Category = "Attacking")
		bool bIsPunching;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

};
