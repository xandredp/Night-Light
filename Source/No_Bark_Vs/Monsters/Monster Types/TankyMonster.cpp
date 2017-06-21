// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/No_Bark_Vs.h"
#include "AI/MyAIController.h"
#include "Player/NBCharacter.h"
#include "Monsters/Monster Types/TankyMonster.h"


ATankyMonster::ATankyMonster()
{
	UsualSpeed = 160.0f;
	ChargingSpeed = 800.0f;
	ChargingCooldown = 1.5;

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
	
	WatchPoint = TEXT("Watch_Socket");

}
void ATankyMonster::BeginPlay()
{
	Super::BeginPlay();

	//Register the function that will be called when the pawn sees the player
	if (PawnSensingComp)
	{
			PawnSensingComp->OnSeePawn.AddDynamic(this, &ATankyMonster::OnSeePlayer);
	}
}

void ATankyMonster::PerformTankyAttack(AActor * HitActor)
{
	if (SoundAttackMelee)
	{
		PlayCharacterSound(SoundAttackMelee);
		SetPlayModeState(EGameModeSoundType::Combat);
	}

	if (GetMonsterDead() == false)
	{
		ANBCharacter* OtherPawn = Cast<ANBCharacter>(HitActor);
		if (OtherPawn)
		{
			LastStrikeTime = GetWorld()->GetTimeSeconds();
		//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Attack!");
			//DamageCharacterHealth();
		}
	}
}

void ATankyMonster::PerformTankyCharge(AActor * HitActor)
{
	if (SoundAttackMelee)
	{
		PlayCharacterSound(SoundAttackCharge);
		SetPlayModeState(EGameModeSoundType::Combat);
	}

	if (GetMonsterDead() == false)
	{
		ANBCharacter* OtherPawn = Cast<ANBCharacter>(HitActor);
		if (OtherPawn)
		{
			LastStrikeTime = GetWorld()->GetTimeSeconds();
		//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Attack!");
			//DamageCharacterHealth();
		}
	}
}

void ATankyMonster::OnSeePlayer(APawn* aPawn)
{
	AMyAIController* AIController = Cast<AMyAIController>(GetController());
	SensedPawn = Cast<ANBCharacter>(aPawn);
	//Set the seen target on the blackboard
	if (AIController && SensedPawn)
	{
		if (GetDistanceTo(SensedPawn) < 1500)
		{
			GLog->Log("Seen");
			AIController->SetSeenTarget(SensedPawn);
			LineTrace();
		}
		else
		{
			GLog->Log("Out of Seeing range");
			MonsterState = EBotBehaviorType::Neutral;
			AIController->ResetSeenTarget();
		}
	}
}
