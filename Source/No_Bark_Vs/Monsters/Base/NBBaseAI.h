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

	/* The thinking part of the brain, steers our zombie and makes decisions based on the data we feed it from the Blackboard */
	/* Assigned at the Character level (instead of Controller) so we may use different zombie behaviors while re-using one controller. */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		class UBehaviorTree* BehaviorTree;

	/* Plays the idle, wandering or hunting sound */
	UPROPERTY(VisibleAnywhere, Category = "Sound")
		UAudioComponent* AudioLoopComp;

	//AI seeing, Sensing component
	UPROPERTY(VisibleAnywhere, Category = "AI")
		class UPawnSensingComponent* PawnSensingComp;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnStun();

	/* Triggered by pawn sensing component when a pawn is spotted */
	/* When using functions as delegates they need to be marked with UFUNCTION(). We assign this function to FSeePawnDelegate */
	UFUNCTION()
		virtual void OnSeePlayer(APawn* Pawn);

	UFUNCTION()
		virtual void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	



	UFUNCTION(BlueprintCallable, Category = "Status")
		void OnDeath();


	//ragdoll physics

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float InDarkMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float InLightMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float InDarkAttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float InLightAttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float Health;



	// inventory interaction functions
	UFUNCTION(BlueprintCallable, Category = "Monster")
		void ReduceHealth(int DamageValue);

	UFUNCTION(BlueprintCallable, Category = "Physics")
		void SetRagdollPhysics(const FName& InBoneName, float PhysicsBlendWeight, bool bNewSimulate, bool bRecover, bool bMeshDead);

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void PlaySound(class USoundCue* SoundToPlay);

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void PlayAnimation(class UAnimMontage* AnimMontageToPlay);

};
