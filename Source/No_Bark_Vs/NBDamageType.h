// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/DamageType.h"
#include "NBDamageType.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API UNBDamageType : public UDamageType
{
	GENERATED_BODY()

		UNBDamageType();

	/* Can player die from this damage type (eg. players don't die from hunger) */
	UPROPERTY(EditDefaultsOnly)
		bool bCanDieFrom;

	/* Damage modifier for headshot damage */
	UPROPERTY(EditDefaultsOnly)
		float HeadDmgModifier;

	UPROPERTY(EditDefaultsOnly)
		float LimbDmgModifier;

public:

	bool GetCanDieFrom();

	float GetHeadDamageModifier();

	float GetLimbDamageModifier();
	
	
};
