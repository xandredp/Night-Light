// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API AMyAIController : public AAIController
{
	GENERATED_BODY()

	// Behaviour tree ref
	UBehaviorTreeComponent* BehaviourComp;

	// Blackboard tree comp ref
	UBlackboardComponent* BlackboardComp;

	//Blackboard keys
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName LocationToGoKey;

	TArray<AActor*> TargetPoints;

	virtual void Possess(APawn* Pawn) override;

public:

	AMyAIController();

	FORCEINLINE UBlackboardComponent* GetBlackBoardComp() const { return BlackboardComp; }

	FORCEINLINE TArray<AActor*> GetAvailableTargetPoints() { return TargetPoints; }
};
