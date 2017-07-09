// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/No_Bark_Vs.h"
#include "AI/MyAIController.h"
#include "Monsters/Base/Monster.h"
#include "AI/PartolTargetPoint.h"
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
	LocationVectorKey = "LocationVector";
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


		//Start the behavior tree which corresponds to the specific character
		BehaviourComp->StartTree(*(AIChar->BehaviorTree));
	}
}

void AMyAIController::SetSensedTarget(APawn* NewTarget)
{
	//Set a new target to follow
	if (BlackboardComp) BlackboardComp->SetValueAsObject(TargetKey, NewTarget);
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
	uint8 temp;

	if (BlackboardComp)
	{
		temp = static_cast<uint8>(NewState);
		BlackboardComp->SetValueAsEnum(AIStateKey, temp);
	}
}

EBotBehaviorType AMyAIController::GetBlackboardBotState()
{
	EBotBehaviorType CurrentState;
	uint8 temp;

	if (BlackboardComp)
	{
		temp = BlackboardComp->GetValueAsEnum(AIStateKey);
		CurrentState = static_cast<EBotBehaviorType>(temp);
		return CurrentState;
	}
	else
	{
		return EBotBehaviorType::Neutral;
	}
}
void AMyAIController::SetLocationVector(FVector LocationVector)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsVector(LocationVectorKey, LocationVector);
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

FVector AMyAIController::GetLocationVector()
{
	if (BlackboardComp)
	{
		return BlackboardComp->GetValueAsVector(LocationVectorKey);
	}
	else
	{
		return(FVector(0, 0, 0));
	}

}

void AMyAIController::SetAIStateFlee()
{
	AMonster* Monster = Cast<AMonster>(GetPawn());
	Monster->MonsterState = EBotBehaviorType::Flee;

	if (BlackboardComp)
	{
		SetBlackboardBotState(Monster->MonsterState);
	}
}

bool AMyAIController::IsAIStateFlee()
{
	if (BlackboardComp)
	{
		return (GetBlackboardBotState() == EBotBehaviorType::Flee);
	}
	else
	{
		return (false);
	}
}


void AMyAIController::SetAIStateStunned()
{
	AMonster* Monster = Cast<AMonster>(GetPawn());
	Monster->MonsterState = EBotBehaviorType::Stunned;

	if (BlackboardComp)
	{
		SetBlackboardBotState(Monster->MonsterState);
	}
}

bool AMyAIController::IsAIStateStunned()
{
	if (BlackboardComp)
	{
		return (GetBlackboardBotState() == EBotBehaviorType::Stunned);
	}
	else
	{
		return (false);
	}
}

void AMyAIController::SetAIStateNeutral()
{
	AMonster* Monster = Cast<AMonster>(GetPawn());
	Monster->MonsterState = EBotBehaviorType::Neutral;
	if (BlackboardComp)
	{
		SetBlackboardBotState(Monster->MonsterState);
	}
}


void AMyAIController::SetAIStateSuspicious()
{
	AMonster* Monster = Cast<AMonster>(GetPawn());
	Monster->MonsterState = EBotBehaviorType::Suspicious;
	if (BlackboardComp)
	{
		SetBlackboardBotState(Monster->MonsterState);
	}
}


// Return an Array of TargetPoints - filtered by WaveTag

TArray<AActor*> AMyAIController::GetAvailableTargetPoints(FName WaveTag)
{

	TArray<AActor*> TargetPointsTag;

	//Populate the array of available target points - using two methods

	//Method 1 - This one succesfully returns all the APartolTargetPoints in the level - BUT the TAGs seem to be empty
	// ActorHasTag(WaveTag) always fails. And manualy inspecting the TargetPoints array in debug shows empty TAGs
	//
	// I want this one fixed - it should be WAY faster than filtering ALL ACTORS by TAG

	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), APartolTargetPoint::StaticClass(), TargetPoints);

	// Method 2 - This one succesfully returns all the APartolTargetPoints in the level - correctly filtered by TAG
	// This is really too slow and inefficient - at least it works

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), WaveTag, TargetPoints);


	// Further filter by WaveTag if needed 
	for (auto It : TargetPoints)
	{

		if (It->ActorHasTag(WaveTag))
		{
			// Add this one to the filtered list 
			TargetPointsTag.Add(It);
		}
		//else
		{
			// Skip this APartolTargetPoint
		}
	}

	return TargetPoints; // Return the filtered list - filtered by WaveTag
}
