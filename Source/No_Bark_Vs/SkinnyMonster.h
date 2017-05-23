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
	
private:
	ASkinnyMonster();

public:
	AMonster* Monster;
	//USkinnyMonsterAnimInstance AnimInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* AttackRangeSphere;
	
	UFUNCTION()
		void OnOverlapWithCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*UFUNCTION()
		void OnEndOverlapWithCharacter(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);*/
};
