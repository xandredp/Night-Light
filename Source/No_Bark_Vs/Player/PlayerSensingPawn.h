// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerSensingPawn.generated.h"

//this class is attached to character with torch. This is used instead of collision of torch

UCLASS()
class NO_BARK_VS_API APlayerSensingPawn : public APawn
{
	GENERATED_BODY()


public:
	// Sets default values for this pawn's properties
	APlayerSensingPawn();

	//Set Owner
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Owning")
		class ANBCharacter* NBCharacter;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//AI seeing, Sensing component
	UPROPERTY(VisibleAnywhere, Category = "AI")
		class UPawnSensingComponent* PawnSensingComp;

	UFUNCTION()
		void OnSeeEnemy(APawn* Pawn);
	//When enemy is stuned spawn particle effect
	UFUNCTION(BlueprintImplementableEvent)
		void SpawnParticleEffect(APawn* Pawn);
};
