// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "SkinnyMonsterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API USkinnyMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool CanAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsAttackFinished;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool CanSeePlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool CanHearPlayer;


protected:

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MovementSpeed;
	
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
		void UpdateAnimationProperties();
};
