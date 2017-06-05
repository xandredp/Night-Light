// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "SkinnyMonsterAnimInstance.h"
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

	AMonster* Monster;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Anims")
		class UBlendSpace1D* RunToAttackBlendSpace;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* AttackRangeSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* AttackAnimTriggerSphere;
	

	UFUNCTION()
		void OnOverlapWithCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlapWithCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void OnOverlapStartAnim(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlapStopAnim(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
		void PerformAttack(AActor* HitActor);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Health")
		void DecreaseCharacterHealth();

private:

	AActor* StoredOtherActor;

	UPROPERTY(EditDefaultsOnly, Category = "Sockets")
		FName AttackAttachPoint;


};
