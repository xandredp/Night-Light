// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NBBaseAI.generated.h"

UCLASS()
class NO_BARK_VS_API ANBBaseAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANBBaseAI();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Physics")
		void SetRagdollPhysics();

	UFUNCTION(BlueprintCallable, Category = "Physics")
		void EnableCollisionCapsuleCollision(bool bEnableCollision);


};
