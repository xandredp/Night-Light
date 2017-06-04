// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "TankyMonster.h"


ATankyMonster::ATankyMonster()
{
	UsualSpeed = 160.0f;
	ChargingSpeed = 600.0f;

	TankyMoveComp = GetCharacterMovement();
	// Adjust jump to make it less floaty
	TankyMoveComp->GravityScale = 1.5f;
	TankyMoveComp->JumpZVelocity = 620;
	TankyMoveComp->bCanWalkOffLedgesWhenCrouching = true;
	TankyMoveComp->MaxWalkSpeedCrouched = 200;
	TankyMoveComp->MaxWalkSpeed = UsualSpeed;

	// Enable movements
	TankyMoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
	TankyMoveComp->GetNavAgentPropertiesRef().bCanFly = true;

	MonsterState = EBotBehaviorType::Neutral;

}

