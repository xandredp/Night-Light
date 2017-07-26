// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/No_Bark_Vs.h"
#include "AI/Behaviour Tree Tasks/BTTargetPointSelection.h"
#include "AI/PartolTargetPoint.h"
#include "AI/MyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/NBGameInstance.h"
#include "Engine/GameInstance.h"

EBTNodeResult::Type UBTTargetPointSelection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AMyAIController* AICon = Cast<AMyAIController>(OwnerComp.GetAIOwner());

	FName WaveTag;
	WaveTag = FName(TEXT("Wave1"));

	UNBGameInstance* SGI = Cast<UNBGameInstance>(GetWorld()->GetGameInstance());
	if (SGI)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, this->GetName() + TEXT(" - level=") + FString::FromInt(SGI->LevelValue));

		switch (SGI->LevelValue)
		{
		case 1:
			WaveTag = FName(TEXT("Wave1"));
			break;
		case 2:
			WaveTag = FName(TEXT("Wave2"));
			break;
		case 3:
			WaveTag = FName(TEXT("Wave3"));
			break;		
		case 4:
			WaveTag = FName(TEXT("Wave4"));
			break;
		default:
		//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Wave index out of range - in UBTTargetPointSelection");
			break;
		}
	}
	if (AICon)
	{
		UBlackboardComponent* BlackBoardComp = AICon->GetBlackBoardComp();

		APartolTargetPoint* CurrentPoint = Cast<APartolTargetPoint>(BlackBoardComp->GetValueAsObject("LocationGoTo"));

		TArray<AActor*> AvailableTargetPoints = AICon->GetAvailableTargetPoints(WaveTag);

		// Contains a random index to define the next target point
		int32 RandomIndex;

		

		//Store the next possible point
		AActor* NextTargetPoint = nullptr;

		if (AvailableTargetPoints.Num() > 0) {

			// Find a next point which is different from the current one
			do
			{
				RandomIndex = FMath::RandRange(0, AvailableTargetPoints.Num() - 1);
				//NextTargetPoint = Cast<APartolTargetPoint>(AvailableTargetPoints[RandomIndex]);
				NextTargetPoint = AvailableTargetPoints[RandomIndex];

			} while (CurrentPoint == NextTargetPoint);

			// Update the next location in the blackboard so the AI can move to the next blackboard value
			BlackBoardComp->SetValueAsObject("LocationToGo", NextTargetPoint);
		}

		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}