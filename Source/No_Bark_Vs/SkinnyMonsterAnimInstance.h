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
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool CanAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsHit;

protected:

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//	bool IsFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MovementSpeed;
	
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
		void UpdateAnimationProperties();



};
