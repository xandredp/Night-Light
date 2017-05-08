// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "BaseEnemy.h"
#include "EnemyAIController.h"

/* AI Specific includes */
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


AEnemyAIController::AEnemyAIController()
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));

	/* Match with the AI/ZombieBlackboard */
	PatrolLocationKeyName = "PatrolLocation";
	CurrentWaypointKeyName = "CurrentWaypoint";
	BotTypeKeyName = "BotType";
	TargetEnemyKeyName = "TargetEnemy";

	/* Initializes PlayerState so we can assign a team index to AI */
	bWantsPlayerState = true;
}

void AEnemyAIController::Possess(class APawn* InPawn)
{
	Super::Possess(InPawn);

	//ABaseEnemy* EnemyBot = Cast<ABaseEnemy>(InPawn);
	//if (EnemyBot)
	//{
	//	if (EnemyBot->BehaviorTree->BlackboardAsset)
	//	{
	//		BlackboardComp->InitializeBlackboard(*EnemyBot->BehaviorTree->BlackboardAsset);
	//	}
	//}

}


void AEnemyAIController::UnPossess()
{
	Super::UnPossess();

}

void AEnemyAIController::SetEnemy(APawn * InPawn)
{
}

void AEnemyAIController::SearchForEnemy()
{
}

//
//void AEnemyAIController::SetBlackboardBotType(EBotBehaviorType NewType)
//{
//	if (BlackboardComp)
//	{
//		BlackboardComp->SetValueAsEnum(BotTypeKeyName, (uint8)NewType);
//	}
//}