// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "AI/MyAIController.h"
#include "SkinnyMonsterAnimInstance.generated.h"


/**
 * 
 */
UCLASS()
class NO_BARK_VS_API USkinnyMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool CanAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsAttackFinished;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool CanCharge;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool CanChargeAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsChargeFinished;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool CanSeePlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool CanHearPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EBotBehaviorType CurrentMonsterState;

protected:

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MovementSpeed;
	
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
		void UpdateAnimationProperties();
};
