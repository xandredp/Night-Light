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

	TargetKeyName = "Target";
	MoveToLocationKeyName = "MoveToLocation";
	AIStateKeyName= "AIState";
	LastDetectedLocationKeyName = "LastDetectedLocation";
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

void ANBAIController::SetTargetKey(APawn* NewTarget)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetKeyName, NewTarget);
	}
}
void ANBAIController::SetAIStateKey(EBotBehaviorType NewState)
{
	uint8 temp;
	CurrentAiState = NewState;
	if (BlackboardComp)
	{
		temp = static_cast<uint8>(NewState);
		BlackboardComp->SetValueAsEnum(AIStateKeyName, temp);
	}
}
EBotBehaviorType ANBAIController::GetAIStateKey()
{
	return CurrentAiState;
}

void ANBAIController::SetMoveToLocationKey(FVector LocationVector)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsVector(MoveToLocationKeyName, LocationVector);
	}
}

void ANBAIController::SetLastDetectedLocationKey(FVector LocationVector)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsVector(LastDetectedLocationKeyName, LocationVector);
	}
}

FVector ANBAIController::GetLastDetectedLocationKey()
{
	if (BlackboardComp)
	{
		return BlackboardComp->GetValueAsVector(LastDetectedLocationKeyName);
	}
	else
	{
		return(FVector(0, 0, 0));
	}
}
