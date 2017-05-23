// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "SkinnyMonsterAnimInstance.h"


void USkinnyMonsterAnimInstance::UpdateAnimationProperties()
{
	// Get the pawn for this anim sequence
	APawn* Pawn = TryGetPawnOwner();

	if (Pawn)
	{
		// Update falling property


		// Update if monster can attack or not
		//CanAttack = false;

		// Update if monster is hit
		IsHit = false;

		// Update movement speed
		MovementSpeed = Pawn->GetVelocity().Size();
	}
}