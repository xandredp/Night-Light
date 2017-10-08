// Fill out your copyright notice in the Description page of Project Settings.
#include "BTRandomMovement.h"
#include "Core/No_Bark_Vs.h"
#include "AI/PartolTargetPoint.h"
#include "AI/MyAIController.h"
#include "Monsters/Base/Monster.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UBTRandomMovement::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	
	AMyAIController* AICon = Cast<AMyAIController>(OwnerComp.GetAIOwner());
	AMonster* AICharacter = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	AICharacter->GetActorLocation();
	
	//if characterisvalid

	if (AICon)
	{
		AICharacter->MonsterState = EBotBehaviorType::Neutral;
		AICon->SetBlackboardBotState(AICharacter->MonsterState);
		
	}
	//UNavigationComponent* PathFindingComp;
	//PathFindingComp->
	//	FindComponentByClass<UNavigationComponent>();
	//if (PathFindingComp != NULL) {
	//	bool bFoundPath = PathFindingComp->FindPathToLocation(dest, NULL);
	//	if (bFoundPath == false || PathFindingComp->GetPath().IsValid() == false) {
	//		bFoundPath = PathFindingComp->FindSimplePathToLocation(dest);
	//	}
	//	if (!bFoundPath) {
	//		// display some error here or whatever
	//	}
	//}

	return EBTNodeResult::Type();
}
