// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API AMyAIController : public AAIController
{
	GENERATED_BODY()

protected:

	/*This property is used to find a certain key for our blackboard.
	We will create the blackboard later in this tutorial*/
	UPROPERTY(EditDefaultsOnly)
		FName SensedTarget = "SensedPawn";
	// Behaviour tree ref
	UBehaviorTreeComponent* BehaviourComp;

	// Blackboard tree comp ref
	UBlackboardComponent* BlackboardComp;

	//Blackboard keys
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName LocationToGoKey;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName TargetKey;


	TArray<AActor*> TargetPoints;

	

public:

	AMyAIController();
	
	//Executes when the controller possess the AI pawn
	virtual void Possess(APawn* Pawn) override;

	//Sets the sensed target in the blackboard
	void SetSeenTarget(APawn* Pawn);

	/*Sets the new sensed target value inside our Blackboard values*/
	void SetSensedTarget(APawn* NewTarget);

	FORCEINLINE UBlackboardComponent* GetBlackBoardComp() const { return BlackboardComp; }

	FORCEINLINE TArray<AActor*> GetAvailableTargetPoints() { return TargetPoints; }
};
