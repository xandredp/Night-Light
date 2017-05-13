// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ModuleManager.h"
#include "EnemyType.h"
#include "AIBaseCharacter.generated.h"

UCLASS()
class AI_PLUGIN_API AAIBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAIBaseCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		EEnemyBehaviourType eItemType;
	
};