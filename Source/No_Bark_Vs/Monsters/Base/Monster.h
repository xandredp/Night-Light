// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Core/TypeClass.h"
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


	/* Properties shared across all derived classes */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float AttackRange;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* SoundPlayerNoticed;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* SoundHunting;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* SoundIdle;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* SoundWandering;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* SoundAttackMelee;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* SoundDeathAttack;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* SoundAttackCharge;
	/* Attack Animation time &  Minimum time between melee attacks */
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		float IdleSoundCooldown;
	/* Minimum time between melee attacks */
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		float LastIdlePlayTime;
	/* Attack Animation time &  Minimum time between melee attacks */
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	float MeleeStrikeCooldown;
	/* Minimum time between melee attacks */
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		float LastStrikeTime;
	/* Plays the idle, wandering or hunting sound */
	UPROPERTY(VisibleAnywhere, Category = "Sound")
		UAudioComponent* AudioLoopComp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EBotBehaviorType MonsterState;

	UFUNCTION(BlueprintImplementableEvent)
		void SetPlayModeState(EGameModeSoundType ChangeSoundState);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		bool bisMonsterDead;

	// inventory interaction functions
	UFUNCTION(BlueprintCallable, Category = "Monster")
		UAudioComponent* PlayCharacterSound(USoundCue* CueToPlay);

	// inventory interaction functions
	UFUNCTION(BlueprintCallable, Category = "Monster")
		void ReduceHealth(int DamageValue);

	UFUNCTION(BlueprintCallable, Category = "Monster")
		bool GetMonsterDead();

	UFUNCTION(BlueprintCallable, Category = "Monster")
		void SetRagdollPhysics();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		bool Fleeing;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Health")
		void DecreaseCharacterHealth();

	UFUNCTION(BlueprintCallable, Category = "Monster")
		void PlayAttackSound();

	UFUNCTION(BlueprintCallable, Category = "Monster")
		void PlayDeathAttackSound();
};
