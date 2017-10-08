// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "AIController.h"
#include "../../Core/TypeClass.h"
#include "NBAIController.generated.h"


/**
 * 
 */
UCLASS()
class NO_BARK_VS_API ANBAIController : public AAIController
{
	GENERATED_BODY()

	ANBAIController();

	/* Called whenever the controller possesses a character bot */
	virtual void Possess(class APawn* InPawn) override;

	virtual void UnPossess() override;

	// Behaviour tree ref
	class UBehaviorTreeComponent* BehaviorComp;
	// Blackboard tree comp ref
	class UBlackboardComponent* BlackboardComp;

	//Blackboard keys

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName TargetKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName AIStateKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName MoveToLocationKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName LastDetectedLocationKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName SelfActorKeyName;
public:
	//sets keys in the blackboard
	UFUNCTION(BlueprintCallable, Category = "BlackBoard")
		void SetTargetKey(APawn* NewTarget);

	//sets keys in the blackboard
	UFUNCTION(BlueprintCallable, Category = "BlackBoard")
		void SetAIStateKey(EBotBehaviorType NewState);

	UFUNCTION(BlueprintCallable, Category = "BlackBoard")
		void SetMoveToLocationKey(FVector LocationVector);

	UFUNCTION(BlueprintCallable, Category = "BlackBoard")
		void SetLastDetectedLocationKey(FVector LocationVector);

	UFUNCTION(BlueprintCallable, Category = "BlackBoard")
		FVector GetLastDetectedLocationKey();

	/** Returns BehaviorComp subobject **/
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorComp() const { return BehaviorComp; }

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
};
