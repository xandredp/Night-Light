// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "CharacterAnimInstance.h"


void UCharacterAnimInstance::UpdateCharacterAnimationProperties()
{
	APawn* Pawn = TryGetPawnOwner();

	if (Pawn)
	{
		CharacterMovementSpeed = Pawn->GetVelocity().Size();
	}
}