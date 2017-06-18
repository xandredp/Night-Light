// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Core/TypeClass.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName AIStateKey;


	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName TargetToChargeKey;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName LocationToCharge;

	TArray<AActor*> TargetPoints;

	

public:

	AMyAIController();
	
	//Executes when the controller possess the AI pawn
	virtual void Possess(APawn* aPawn) override;

	//Sets the sensed target in the blackboard
	// inventory interaction functions
	UFUNCTION(BlueprintCallable, Category = "BlackBoard")
	void SetSeenTarget(APawn* aPawn);

	UFUNCTION(BlueprintCallable, Category = "BlackBoard")
	void SetBlackboardBotState(EBotBehaviorType NewState);

	UFUNCTION(BlueprintCallable, Category = "BlackBoard")
		void SetLocationToCharge(FVector LocationVector);
	
	UFUNCTION(BlueprintCallable, Category = "BlackBoard")
	void ResetSeenTarget();

	UFUNCTION(BlueprintCallable, Category = "BlackBoard")
		void ResetChargeTarget();

	/*Sets the new sensed target value inside our Blackboard values*/
	UFUNCTION(BlueprintCallable, Category = "BlackBoard")
	void SetSensedTarget(APawn* NewTarget);

	UFUNCTION(BlueprintCallable, Category = "BlackBoard")
		void SetChargeTarget(APawn* aPawn);


	UFUNCTION(BlueprintCallable, Category = "BlackBoard")
	FVector GetLocationToCharge();

	UFUNCTION(BlueprintCallable, Category = "BlackBoard")
		EBotBehaviorType GetBlackboardBotState();

	UFUNCTION(BlueprintCallable, Category = "BlackBoard")
		void SetAIStateFlee();

	FORCEINLINE UBlackboardComponent* GetBlackBoardComp() const { return BlackboardComp; }

	FORCEINLINE TArray<AActor*> GetAvailableTargetPoints() { return TargetPoints; }
};
