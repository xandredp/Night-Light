// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "SkinnyMonsterAnimInstance.h"


void USkinnyMonsterAnimInstance::UpdateAnimationProperties()
{
	// Get the pawn for this anim sequence
	APawn* Pawn = TryGetPawnOwner();

	if (Pawn)
	{
		// Update movement speed
		MovementSpeed = Pawn->GetVelocity().Size();
	}
}