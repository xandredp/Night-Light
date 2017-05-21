// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "BTTargetPointSelection.h"
#include "PartolTargetPoint.h"
#include "MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTargetPointSelection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyAIController* AICon = Cast<AMyAIController>(OwnerComp.GetAIOwner());

	if (AICon)
	{
		UBlackboardComponent* BlackBoardComp = AICon->GetBlackBoardComp();

		APartolTargetPoint* CurrentPoint = Cast<APartolTargetPoint>(BlackBoardComp->GetValueAsObject("LocationGoTo"));

		TArray<AActor*> AvailableTargetPoints = AICon->GetAvailableTargetPoints();

		// Contains a random index to define the next target point
		int32 RandomIndex;

		

		//Store the next possible point
		APartolTargetPoint* NextTargetPoint = nullptr;

		// Find a next point which is different from the current one
		do
		{
			RandomIndex = FMath::RandRange(0, AvailableTargetPoints.Num() - 1);
			NextTargetPoint = Cast<APartolTargetPoint>(AvailableTargetPoints[RandomIndex]);
		} while (CurrentPoint == NextTargetPoint);

		// Update the next location in the blackboard so the AI can move to the next blackboard value
		BlackBoardComp->SetValueAsObject("LocationToGo", NextTargetPoint);

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}