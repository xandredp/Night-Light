// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "Monster/Monster.h"
#include "MyAIController.h"
#include "NBCharacter.h"
#include "GameFramework/GameMode.h"
#include "TypeClass.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"

//Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initializing the pawn sensing component
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	// Set the peripheral vision angle to 80 degrees
	PawnSensingComp->SetPeripheralVisionAngle(80.0f);

	MonsterState = EBotBehaviorType::Neutral;
	Fleeing = false;
	bisMonsterDead = false;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
	//Register the function that will be called when the pawn sees the player
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AMonster::OnSeePlayer);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &AMonster::OnHearNoise);
	}
}

void AMonster::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	//AMyAIController* Con = Cast<AMyAIController>(GetController());

	//We don't want to hear ourselves
	//if (Con && PawnInstigator != this)
	//{
	//	//Updates our target based on what we've heard.
	//	//Con->SetSensedTarget(PawnInstigator);
	//}
}

void AMonster::OnSeePlayer(APawn* aPawn)
{
	if (Health <= 0.0)
	{
		bisMonsterDead = false;
		return;
	}

	AMyAIController* AIController = Cast<AMyAIController>(GetController());
	ANBCharacter* SensedPawn = Cast<ANBCharacter>(aPawn);
	//Set the seen target on the blackboard
	if (AIController && SensedPawn)
	{
		if (GetDistanceTo(SensedPawn) < 1500)
		{
			GLog->Log("Seen");
			MonsterState = EBotBehaviorType::Suspicious;
			AIController->SetBlackboardBotState(MonsterState);

			AIController->SetSeenTarget(SensedPawn);

			if (SoundPlayerNoticed)
			{
				PlayCharacterSound(SoundPlayerNoticed);
				SetPlayModeState(EGameModeSoundType::Alert);
			}
		}
		else
		{	
			GLog->Log("Out of Seeing range");
			MonsterState = EBotBehaviorType::Neutral;
			AIController->SetBlackboardBotState(MonsterState);
			if (SoundIdle)
			{
				PlayCharacterSound(SoundIdle);
				SetPlayModeState(EGameModeSoundType::General);
			}
			AIController->ResetSeenTarget();

		}
	}
}


// Called every frame
void AMonster::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AMonster::ReduceHealth(int DamageValue)
{
	if (bisMonsterDead == false)
	{
		Health = Health - DamageValue;

		if (Health <= 0)
		{
			bisMonsterDead = true;
			SetRagdollPhysics();
		}
		else
		{

		}
	}
}

bool AMonster::GetMonsterDead()
{
	if (Health <= 0)
	{
		bisMonsterDead = true;
		SetRagdollPhysics();
	}
	else
	{
		bisMonsterDead = false;
	}

	return bisMonsterDead;
}

void AMonster::SetRagdollPhysics()
{
	bool bInRagdoll = false;
	USkeletalMeshComponent* Mesh3P = GetMesh();

	if (IsPendingKill())
	{
		bInRagdoll = false;
	}
	else if (!Mesh3P || !Mesh3P->GetPhysicsAsset())
	{
		bInRagdoll = false;
	}
	else
	{
		Mesh3P->SetAllBodiesSimulatePhysics(true);
		Mesh3P->SetSimulatePhysics(true);
		Mesh3P->WakeAllRigidBodies();
		Mesh3P->bBlendPhysics = true;

		bInRagdoll = true;
	}

	UCharacterMovementComponent* CharacterComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (CharacterComp)
	{
		CharacterComp->StopMovementImmediately();
		CharacterComp->DisableMovement();
		CharacterComp->SetComponentTickEnabled(false);
	}

	if (!bInRagdoll)
	{
		// Immediately hide the pawn
		TurnOff();
		SetActorHiddenInGame(true);
		SetLifeSpan(1.0f);
	}
	else
	{
		SetLifeSpan(10.0f);
	}
}
UAudioComponent* AMonster::PlayCharacterSound(USoundCue* CueToPlay)
{
	if (CueToPlay)
	{
		return UGameplayStatics::SpawnSoundAttached(CueToPlay, RootComponent, NAME_None, FVector::ZeroVector, EAttachLocation::SnapToTarget, true);
	}

	return nullptr;

}