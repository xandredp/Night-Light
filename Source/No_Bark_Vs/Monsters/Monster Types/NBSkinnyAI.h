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




	/************************************************************************/
	/* Animation & Sounds with Animation                                    */
	/************************************************************************/

	//void Tick(float DeltaSeconds) override;
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
		class USoundCue* SoundIdle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
		class UAnimMontage* AttackAnimation;

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

private:

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName RightHandSocket;
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName LeftHandSocket;
	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName HeadSocket;

	
};
