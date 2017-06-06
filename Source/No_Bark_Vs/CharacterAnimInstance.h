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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CharacterMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsAiming;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsPrimaryEquiped;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsCurrentWeaponHeld;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsSprinting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsCrouched;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsFiring;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsReloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsIronSighting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FRotator AimOffsetRot;
	
protected:
	UFUNCTION(BlueprintCallable, Category = "Update character Animation Properties")
		void UpdateCharacterAnimationProperties();
};
