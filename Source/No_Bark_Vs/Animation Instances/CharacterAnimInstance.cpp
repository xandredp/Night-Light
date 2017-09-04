// Fill out your copyright notice in the Description page of Project Settings.
#include "CharacterAnimInstance.h"
#include "Core/No_Bark_Vs.h"



void UCharacterAnimInstance::UpdateCharacterAnimationProperties()
{
	APawn* Pawn = TryGetPawnOwner();

	if (Pawn)
	{
		CharacterMovementSpeed = Pawn->GetVelocity().Size();
	}
}