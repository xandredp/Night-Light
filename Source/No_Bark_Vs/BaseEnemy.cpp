// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "EnemyAIController.h"
#include "BaseEnemy.h"


ABaseEnemy::ABaseEnemy()
{
	/* Ignore this channel or it will absorb the trace impacts instead of the skeletal mesh */
	GetCapsuleComponent()->SetCollisionResponseToChannel(WEAPON_TRACE, ECR_Ignore);
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f, false);
	GetCapsuleComponent()->SetCapsuleRadius(42.0f);

	Health = 100;

}