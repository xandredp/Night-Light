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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsPrimaryEquiped;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsSprinting;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Update character Animation Properties")
		void UpdateCharacterAnimationProperties();
};
