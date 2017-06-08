// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "MyAIController.h"
#include "Monster/Monster.h"
#include "PartolTargetPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"

AMyAIController::AMyAIController()
{
	// initialize behaviorTreeComponent, BlackBoardComponent and the corresponding keys
	BehaviourComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));

	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	LocationToGoKey = "LocationToGo";
	AIStateKey = "AIState";
	TargetKey = "Target";
	LocationToCharge = "LocationVector";
	TargetToChargeKey = "ChargeTarget";

}

void AMyAIController::Possess(APawn* aPawn)
{
	Super::Possess(aPawn);

	//Get the possessed character and check if its my own AI character
	AMonster* AIChar = Cast<AMonster>(aPawn);

	if (AIChar)
	{
		// if the blackboard is valid initialize the blackboard for the corresponding behavior tree
		if (AIChar->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(AIChar->BehaviorTree->BlackboardAsset));
		}

		//Populate the array of available target points
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APartolTargetPoint::StaticClass(), TargetPoints);

		//Start the behavior tree which corresponds to the specific character
		BehaviourComp->StartTree(*(AIChar->BehaviorTree));
	}
}

void AMyAIController::SetSensedTarget(APawn* NewTarget)
{
	//Set a new target to follow
	if (BlackboardComp) BlackboardComp->SetValueAsObject(SensedTarget, NewTarget);
}



void AMyAIController::SetSeenTarget(APawn* aPawn)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetKey, aPawn);
	}
}

void AMyAIController::SetChargeTarget(APawn* aPawn)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetToChargeKey, aPawn);
	}
}

void AMyAIController::SetBlackboardBotState(EBotBehaviorType NewState)
{
	FName CurrentState = "NULL";

	if (NewState == EBotBehaviorType::Neutral)
	{
		CurrentState = "Neutral";

	}
	else if (NewState == EBotBehaviorType::Suspicious)
	{
		CurrentState = "Suspicious";
	}
	else if (NewState == EBotBehaviorType::Flee)
	{
		CurrentState = "Flee";
	}
	else if (NewState == EBotBehaviorType::Agression)
	{
		CurrentState = "Agression";
	}
	else if (NewState == EBotBehaviorType::Charge)
	{
		CurrentState = "Charge";
	}
	else if (NewState == EBotBehaviorType::Stunned)
	{
		CurrentState = "Stunned";
	}
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsName(AIStateKey, CurrentState);
	}
}

void AMyAIController::SetLocationToCharge(FVector LocationVector)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsVector(LocationToCharge, LocationVector);
	}
}

void AMyAIController::ResetSeenTarget()
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetKey, nullptr);
	}
}
void AMyAIController::ResetChargeTarget()
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetToChargeKey, nullptr);
	}
}

FVector AMyAIController::GetLocationToCharge()
{
	if (BlackboardComp)
	{
		return BlackboardComp->GetValueAsVector(LocationToCharge);
	}
	else
	{
		return(FVector(0, 0, 0));
	}

}

void AMyAIController::SetAIStateFlee()
{
	FName CurrentState = "NULL";
	
	CurrentState = "Flee";

	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsName(AIStateKey, CurrentState);
	}
}