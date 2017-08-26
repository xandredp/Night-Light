// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monsters/Base/Monster.h"
//#include "Animation Instances/SkinnyMonsterAnimInstance.h"
#include "SkinnyMonster.generated.h"


/**
 * 
 */
UCLASS()
class NO_BARK_VS_API ASkinnyMonster : public AMonster
{
	GENERATED_BODY()

	/* Timer handle to manage continous melee attacks while in range of a player */
	FTimerHandle TimerHandle_MeleeAttack;


public:
	ASkinnyMonster();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	AMonster* Monster;

	UAnimInstance* SkinnyMonsterAnimInstance;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		class USkeletalMeshComponent* SkinnyMonsterSkeletal;


	// Sphere component attached to the hand 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* AttackRangeSphere;

	// Sphere component around monster
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* AttackAnimTriggerSphere;

	/* UPROPERTY(EditDefaultsOnly, Category = "Anims")
	USkinnyMonsterAnimInstance* AnimInstance; */
	/************************************************************************/
	/* Overlap functions                                      */
	/************************************************************************/

	UFUNCTION()
		void OnOverlapWithCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlapWithCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void OnOverlapStartAnim(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlapStopAnim(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/************************************************************************/
	/*Attacks				                                                */
	/************************************************************************/

	UFUNCTION()
		void PerformAttack(AActor* HitActor);

	UFUNCTION()
		void PerformStunned();

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		class UAnimMontage* AttackAnimMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animations")
		class UAnimMontage* StunAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		bool IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
		bool StunnedAnimPlaying;




private:

	AActor* StoredOtherActor;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName AttackAttachPoint;


};
