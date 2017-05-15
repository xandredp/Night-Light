// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "NBDamageType.h"



UNBDamageType::UNBDamageType()
{
	/* We apply this modifier based on the physics material setup to the head of the enemy PhysAsset */
	HeadDmgModifier = 2.0f;
	LimbDmgModifier = 0.5f;

	bCanDieFrom = true;
}


bool UNBDamageType::GetCanDieFrom()
{
	return bCanDieFrom;
}


float UNBDamageType::GetHeadDamageModifier()
{
	return HeadDmgModifier;
}

float UNBDamageType::GetLimbDamageModifier()
{
	return LimbDmgModifier;
}


