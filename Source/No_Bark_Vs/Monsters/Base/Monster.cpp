// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/No_Bark_Vs.h"
#include "Monsters/Base/Monster.h"
#include "AI/MyAIController.h"
#include "Player/NBCharacter.h"
#include "GameFramework/GameMode.h"
#include "Core/TypeClass.h"
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

	AudioLoopComp = CreateDefaultSubobject<UAudioComponent>(TEXT("ZombieLoopedSoundComp"));
	AudioLoopComp->bAutoActivate = false;
	AudioLoopComp->bAutoDestroy = false;
	AudioLoopComp->SetupAttachment(RootComponent);

	MonsterState = EBotBehaviorType::Neutral;
	Fleeing = false;
	bisMonsterDead = false;
	LastIdlePlayTime = 0.0f;
	IdleSoundCooldown = 1.0f;
	MonsterValue = 100.0f;
	bisScoreAdded = false;
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
	if (SoundIdle)
	{
		PlayCharacterSound(SoundIdle);
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
	EBotBehaviorType PreMonsterState;
	PreMonsterState = MonsterState;
	if (Health <= 0.0)
	{
		bisMonsterDead = false;
		return;
	}

	AMyAIController* AIController = Cast<AMyAIController>(GetController());
	SensedPawn = Cast<ANBCharacter>(aPawn);
	//Set the seen target on the blackboard
	if (AIController && SensedPawn)
	{
		if (GetDistanceTo(SensedPawn) < 1500)
		{
			GLog->Log("Seen");
			MonsterState = EBotBehaviorType::Suspicious;
			AIController->SetBlackboardBotState(MonsterState);

			AIController->SetSeenTarget(SensedPawn);

			//When changed to suspicious cry once. 
			if (PreMonsterState != MonsterState)
			{
				if (SoundPlayerNoticed)
				{
					//PlayCharacterSound(SoundPlayerNoticed);
					AudioLoopComp->SetSound(SoundPlayerNoticed);
					AudioLoopComp->Play();
					SetPlayModeState(EGameModeSoundType::Alert);
				}
			}
			
		}
		else
		{	
			GLog->Log("Out of Seeing range");
			MonsterState = EBotBehaviorType::Neutral;
			AIController->SetBlackboardBotState(MonsterState);
			if (SoundIdle)
			{
				AudioLoopComp->SetSound(SoundIdle);
				AudioLoopComp->Play();
				LastIdlePlayTime = GetWorld()->GetTimeSeconds();
				SetPlayModeState(EGameModeSoundType::General);
			}
			/*if (GetWorld()->TimeSeconds - LastIdlePlayTime > IdleSoundCooldown)
			{
				if (SoundIdle)
				{
					AudioLoopComp->SetSound(SoundIdle);
					AudioLoopComp->Play();
					LastIdlePlayTime = GetWorld()->GetTimeSeconds();
					SetPlayModeState(EGameModeSoundType::General);
				}
			}*/


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
			if (bisScoreAdded == false)
			{
				SensedPawn->IncreaseScore(MonsterValue);
				bisScoreAdded = true;
			}
		
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
void AMonster::PlayAttackSound()
{
	if (SoundAttackMelee)
	{
		AudioLoopComp->SetSound(SoundAttackMelee);
		AudioLoopComp->Play();
		SetPlayModeState(EGameModeSoundType::Combat);
	}
}
void AMonster::PlayDeathAttackSound()
{
	if (SoundDeathAttack)
	{
		AudioLoopComp->SetSound(SoundDeathAttack);
		AudioLoopComp->Play();
		SetPlayModeState(EGameModeSoundType::Death);
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