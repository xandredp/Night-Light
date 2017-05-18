// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
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
	//USphereCollisionComponent* AttackRangeSphere;
	
};
