// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "NBCharacter.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */

UCLASS()
class NO_BARK_VS_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	

public:
	// Sets default values for this character's properties
	AEnemyAIController();


	/* Called whenever the controller possesses a character bot */
	virtual void Possess(class APawn* InPawn) override;

	virtual void UnPossess() override;

	void SetEnemy(class APawn *InPawn);

	UFUNCTION(BlueprintCallable, Category = "AIBehavior")
		virtual void SearchForEnemy();

	UBehaviorTreeComponent* BehaviorComp;

	UBlackboardComponent* BlackboardComp;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName TargetEnemyKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName PatrolLocationKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName CurrentWaypointKeyName;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		FName BotTypeKeyName;

	
};
