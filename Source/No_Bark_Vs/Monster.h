// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "TypeClass.h"
#include "GameFramework/Character.h"
#include "Monster.generated.h"


UCLASS()
class NO_BARK_VS_API AMonster : public ACharacter
{
	GENERATED_BODY()

private:
	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

public:

	/*Hearing function - will be executed when we hear a Pawn*/
	UFUNCTION()
		void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

	// Sets default values for this character's properties
	AMonster();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//AI seeing, Sensing component
	UPROPERTY(VisibleAnywhere, Category = "AI")
	class UPawnSensingComponent* PawnSensingComp;

	// The behaviour tree of the character
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere)
	float MovementSpeed;

	UPROPERTY(EditAnywhere)
	float Health;

	UPROPERTY(EditAnywhere)
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EBotBehaviorType MonsterState;

	void ReduceHealth();
};
