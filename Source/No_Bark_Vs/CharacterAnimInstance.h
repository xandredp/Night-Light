// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "CharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API UCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float CharacterMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsAiming;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "UpdateAnimationProperties")
		void UpdateCharacterAnimationProperties();
};
