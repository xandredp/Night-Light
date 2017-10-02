// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monsters/Base/NBBaseAI.h"
#include "NBSkinnyAI.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API ANBSkinnyAI : public ANBBaseAI
{
	GENERATED_BODY()
	
		
	/* Timer handle to manage continous melee attacks while in range of a player */
	FTimerHandle TimerHandle_MeleeAttack;

	
public:
	ANBSkinnyAI();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//void Tick(float DeltaSeconds) override;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		class USoundCue* SoundIdle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		bool IsAnimPlaying;

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void SimulateMeleeStrike();
	
};
