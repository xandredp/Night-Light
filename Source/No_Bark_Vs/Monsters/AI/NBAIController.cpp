// Fill out your copyright notice in the Description page of Project Settings.

#include "NBAIController.h"
#include "../Base/NBBaseAI.h"
/* AI Specific includes */
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


ANBAIController::ANBAIController()
{
	// initialize behaviorTreeComponent, BlackBoardComponent and the corresponding keys
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	MoveToLocationKeyName = "MoveToLocation";
	AIStateKeyName = "AIState";
}

void ANBAIController::Possess(APawn * aPawn)
{
	Super::Possess(aPawn);

	//Get the possessed character and check if its my own AI character
	class ANBBaseAI* NBBaseAI = Cast<ANBBaseAI>(aPawn);

	if (NBBaseAI)
	{
		// if the blackboard is valid initialize the blackboard for the corresponding behavior tree
		if (NBBaseAI->BehaviorTree)
		{
			if (NBBaseAI->BehaviorTree->BlackboardAsset)
			{
				BlackboardComp->InitializeBlackboard(*(NBBaseAI->BehaviorTree->BlackboardAsset));
				//Start the behavior tree which corresponds to the specific character
				BehaviorComp->StartTree(*(NBBaseAI->BehaviorTree));

			}
		}
	}

}

void ANBAIController::UnPossess()
{
	Super::UnPossess();

	/* Stop any behavior running as we no longer have a pawn to control */
	BehaviorComp->StopTree();
}

void ANBAIController::SetMoveToLocationKey(FVector LocationVector)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsVector(MoveToLocationKeyName, LocationVector);
	}
}

void ANBAIController::SetCurrentAIState(EBotBehaviorType NewState)
{
	uint8 temp;

	if (BlackboardComp)
	{
		temp = static_cast<uint8>(NewState);
		BlackboardComp->SetValueAsEnum(AIStateKeyName, temp);
	}
}