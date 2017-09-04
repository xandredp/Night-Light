// Fill out your copyright notice in the Description page of Project Settings.
#include "SkinnyMonsterAnimInstance.h"
#include "Core/No_Bark_Vs.h"



void USkinnyMonsterAnimInstance::UpdateAnimationProperties()
{
	// Get the pawn for this anim sequence
	APawn* Pawn = TryGetPawnOwner();

	if (Pawn)
	{
		// Update movement speed
		MovementSpeed = Pawn->GetVelocity().Size();

		AMyAIController* AIController = Cast<AMyAIController>(Pawn->GetController());

		if (AIController) {
			CurrentMonsterState = AIController->GetBlackboardBotState();
		}
	}
}