// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster/SkinnyMonster.h"
#include "TankyMonster.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API ATankyMonster : public ASkinnyMonster
{
	GENERATED_BODY()

	//reduce stamina
	FTimerHandle ChargingTimerHandle;
	//gain stamina
	FTimerHandle StopSprintingTimerHandle;
private:
	UFUNCTION()
		void OnSeePlayer(APawn* Pawn);
public:
	ATankyMonster();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float UsualSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float ChargingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	UCharacterMovementComponent* TankyMoveComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float LastChargingTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float ChargingCooldown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Socket)
		FName WatchPoint;
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Trace")
		void LineTrace();
};
