// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/No_Bark_Vs.h"
#include "BTRandomMovement.h"
#include "AI/PartolTargetPoint.h"
#include "AI/MyAIController.h"
#include "Monsters/Base/Monster.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTRandomMovement::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	
	AMyAIController* AICon = Cast<AMyAIController>(OwnerComp.GetAIOwner());
	AMonster* AICharacter = Cast<AMonster>(OwnerComp.GetAIOwner()->GetPawn());

	//if characterisvalid

	if (AICon)
	{
		AICharacter->MonsterState = EBotBehaviorType::Neutral;
		AICon->SetBlackboardBotState(AICharacter->MonsterState);
	}


	return EBTNodeResult::Type();
}
