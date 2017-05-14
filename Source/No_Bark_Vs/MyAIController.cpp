// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "MyAIController.h"
#include "Monster.h"
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

	TargetKey = "Target";
}

void AMyAIController::Possess(APawn* Pawn)
{
	Super::Possess(Pawn);

	//Get the possessed character and check if its my own AI character
	AMonster* AIChar = Cast<AMonster>(Pawn);

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

void AMyAIController::SetSeenTarget(APawn* Pawn)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetKey, Pawn);
	}
}