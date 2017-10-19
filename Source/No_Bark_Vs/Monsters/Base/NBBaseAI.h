// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../../Core/SoundBlockingActor.h"
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
	//Action Just before Aggression
	virtual void OnReact();

	/* Triggered by pawn sensing component when a pawn is spotted */
	/* When using functions as delegates they need to be marked with UFUNCTION(). We assign this function to FSeePawnDelegate */
	UFUNCTION()
		virtual void OnSeePlayer(APawn* Pawn);

	UFUNCTION()
		virtual void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);
	
	UFUNCTION()
		virtual void OnShotAt();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* Bool to Check in Onseeplayer when player is seen this used to gether with first seen time variable
	to un suspect the monster*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		bool bIsSuspicious;

	/* Bool to get time LastSeenTime - FirstSeenTime (during thhis period) to get the SeenTime
	This is used to detect how hard it is to detect the player*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float FirstDetectedTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float LastDetectedTime;

	/* Maximum Timetaken to detect player This decides how fast enemy is Seen */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float DetectionMaxTime;

	/* Time-out value to clear the suspision
	Should be higher than Sense interval in the PawnSense component not never miss sense ticks.
	Once detectected duration of this much will be following the player*/
	UPROPERTY(EditDefaultsOnly, Category = "AI")
		float SenseTimeOut;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
		float AgrresionTimeOut;

public:	
	// retrns sound blocking actor. checkes if sound blocking actor is in between player and self
	UFUNCTION(BlueprintCallable, Category = "AI")
		ASoundBlockingActor* GetSoundBlockingActorInView();

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
		float CurrentAttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float MaxHealth;

	UFUNCTION(BlueprintCallable, Category = "Monster")
		bool GetMonsterDead();

	// Apply damage to point damage 
	UFUNCTION(BlueprintCallable, Category = "Monster")
		void ApplyDamage(AActor* DamagedActor, float BaseDamage,
			FVector const& HitFromDirection,
			FHitResult const& HitInfo, AController* EventInstigator,
			AActor* DamageCauser,
			TSubclassOf<UDamageType> DamageTypeClass);
	

	UFUNCTION(BlueprintCallable, Category = "Monster")
		void ReduceHealth(int DamageValue);

	UFUNCTION(BlueprintCallable, Category = "Physics")
		void SetRagdollPhysics(const FName& InBoneName, float PhysicsBlendWeight, bool bNewSimulate, bool bRecover, bool bMeshDead);

	UFUNCTION(BlueprintCallable, Category = "Monster")
		void SetWalkSpeed(float desiredWalkSpeed);

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void PlaySound(class USoundCue* SoundToPlay);

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void PlayAnimation(class UAnimMontage* AnimMontageToPlay);


	/************************************************************************/
	/* Light                                            */
	/************************************************************************/

	/* Checks If Monster is in Lighth (Generator) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		bool bisMonsterInLight;

	/* Checks If Monster can be killable in torchActivation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		bool bisMonsterkillableAtStun;
	/* Checks If Monster is in Lighth (Generator) This is used on Blueprint For easy Twick*/

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Status")
		void MonsterInLight(bool isinLight);

	/************************************************************************/
	/* Drop Items                                                */
	/************************************************************************/

	//Primary Weapon Held Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		TSubclassOf <class ABaseInteractable> DropItemOnDeath;

	//Primary Weapon Held Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		class AKeyForDoor* DropKeyOnDeath;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Health")
		void SpawnDropItems();




};
