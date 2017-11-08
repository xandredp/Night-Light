// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "Monsters/Base/NBBaseAI.h"

#include "NBSkinnyAI.generated.h"

/**
 * 
 */

UENUM()
enum EAttackValue
{
	LHand,
	RHand,
	Head
};


UCLASS()
class NO_BARK_VS_API ANBSkinnyAI : public ANBBaseAI
{
	GENERATED_BODY()
	
		
	/* Timer handle to manage continous melee attacks while in range of a player */
		FTimerHandle TimerHandle_MeleeAttack;

	/* Timer handle to manage seen time ticks to get player unseen*/
	FTimerHandle TimerHandle_CountUnSeenTime;
	
protected:
	ANBSkinnyAI(const FObjectInitializer& ObjectInitializer);
public:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnStun() override;

	virtual void OnReact() override;

	UFUNCTION()
		virtual void OnSeePlayer(APawn* Pawn);

	UFUNCTION()
		virtual void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

	UFUNCTION()
		virtual void OnShotAt();

	UFUNCTION()
		void OnLostPlayer();

	/*counting howlong the player has been undetected for if long calms the monster*/
	UFUNCTION(BlueprintCallable, Category = "Monster")
		void CountingPlayerUndetectedTime();

	/************************************************************************/
	/* Animation & Sounds with Animation                                    */
	/************************************************************************/

	//void Tick(float DeltaSeconds) override;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		class USoundCue* SoundIdle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* AttackWithRightHandAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* AttackWithLeftHandAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* AttackWithHeadAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* ReactAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* SearchAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* StunAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		bool IsAnimPlaying;

	UFUNCTION(BlueprintCallable, Category = "Animation")
		void SimulateMeleeStrike();


	/* UPROPERTY(EditDefaultsOnly, Category = "Anims")
	USkinnyMonsterAnimInstance* AnimInstance; */
	/************************************************************************/
	/* Overlap functions                                      */
	/************************************************************************/

	// Sphere component around monster this triggers attack animation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* AttackRangeAnimationTriggerSphere;

	// Sphere component attached to the hand 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* HeadStrikePlayerSphere;

	// Sphere component attached to the hand 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* LeftHandStrikePlayerSphere;

	// Sphere component attached to the hand 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* RightHandStrikePlayerSphere;

	UFUNCTION()
		void OnOverlapStrikeCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlapStrikeCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnOverlapStartAnim(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlapStopAnim(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		bool IsEnemyInCircle;
	/************************************************************************/
	/* Status Change variables & Functions          */
	/************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		float NeutralWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		float SuspiciousWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		float AggressionWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		float ChargeWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		float StunnedWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		float FleeWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		float MinAgressiveDistant;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		class ANBCharacter* NBPlayerCharacter;


	UFUNCTION(BlueprintCallable, Category = "Monster")
		void SetTranparentMaterial();


private:

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName RightHandSocket;
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName LeftHandSocket;
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName HeadSocket;


protected:
	//get correct attack animation using Eattackvalue
	UFUNCTION(BlueprintCallable, Category = "Animation")
		UAnimMontage* GetAttackAnim(EAttackValue AttackType);

	//Set AI State  using EBotBehaviorType
	UFUNCTION(BlueprintCallable, Category = "AI")
		void SetAIState(EBotBehaviorType AIState);
	UFUNCTION(BlueprintCallable, Category = "AI")
		void SetWalkSpeedbyCurrentStatus();
};
