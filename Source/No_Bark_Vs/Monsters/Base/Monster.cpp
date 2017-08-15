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
	PawnSensingComp->SetPeripheralVisionAngle(190.0f);
	PawnSensingComp->SightRadius = 2500;
	PawnSensingComp->HearingThreshold = 600;
	PawnSensingComp->LOSHearingThreshold = 1000;
	MaxVisibleRange = 1500;

	AudioLoopComp = CreateDefaultSubobject<UAudioComponent>(TEXT("ZombieLoopedSoundComp"));
	AudioLoopComp->bAutoActivate = false;
	AudioLoopComp->bAutoDestroy = false;
	AudioLoopComp->SetupAttachment(RootComponent);

	MonsterState = EBotBehaviorType::Neutral;

	DebugDrawEnabledAI = false;
	DebugDrawEnabledAI2 = false;

	Fleeing = false;
	bisMonsterDead = false;
	LastIdlePlayTime = 0.0f;
	IdleSoundCooldown = 1.0f;
	MonsterValue = 100;
	bisScoreAdded = false;
	AfterDeathAutoDelete = 30; // Seconds
	bisMonsterInLight = false;
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
	MonsterState = EBotBehaviorType::Neutral;
	AMyAIController* AIController = Cast<AMyAIController>(GetController());
	AIController->SetBlackboardBotState(MonsterState);

	// Check if monster is in light every 1 second. 
}


//	Tick is mainly present to allow PawnSensingComp to show DrawDebugCone and DrawDebugSphere 
void AMonster::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (GetMonsterDead() == false)
	{
		AMyAIController* AIController = Cast<AMyAIController>(GetController());
		APawn* AIPawn = AIController->GetPawn();
		if (DebugDrawEnabledAI2)
		{
			DrawDebugCone(GetWorld(), AIController->GetPawn()->GetActorLocation(), AIPawn->GetActorForwardVector(), this->PawnSensingComp->SightRadius, (this->PawnSensingComp->GetPeripheralVisionAngle() * (3.14159265 / 180)), (this->PawnSensingComp->GetPeripheralVisionAngle() * (3.14159265 / 180)), 40, FColor::Purple, false, 0.05, 1, 0.5);
			DrawDebugSphere(GetWorld(), AIController->GetPawn()->GetActorLocation(), this->PawnSensingComp->LOSHearingThreshold, 40, FColor::Yellow, false, 0.05, 0, 0.5f);
		}
	}
	else
	{
		float DeathTimer = GetLifeSpan();
		
		if ((DeathTimer > 0) && (DeathTimer > (AfterDeathAutoDelete-0.05))) {
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Death timer started for " + GetName() + " : GetLifeSpan=" + FString::SanitizeFloat(DeathTimer)); 
			AudioLoopComp->Stop(); // recently dead - so stop with the infernal moaning PLEASE!!!
		}
	}


}


//	Hearing - limited to PawnSensingComp->LOSHearingThreshold
void AMonster::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	AMyAIController* AIController = Cast<AMyAIController>(GetController());
	if (GetMonsterDead() == false)
	{
		//We don't want to hear ourselves
		if (AIController && PawnInstigator != this)
		{
			//	//Updates our target based on what we've heard.
			//Con->SetSensedTarget(PawnInstigator);
			float MaxHearingRange = PawnSensingComp->LOSHearingThreshold;
			float Length = (Location - AIController->GetPawn()->GetActorLocation()).Size();

			if (Length > MaxHearingRange)
			{
				// outside of max hearing range
				if (DebugDrawEnabledAI) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, GetName() + " - Too far to hear sound"); }
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Hearing test for " + GetName() + " : MaxHearingRange=" + FString::SanitizeFloat(MaxHearingRange) + " Length=" + FString::SanitizeFloat(Length));
				if (DebugDrawEnabledAI) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, this->GetName() + TEXT(" - AI detected a Noise!")); }
				APawn* aPlayerCharacter = GetWorld()->GetFirstPlayerController()->GetPawn();
				AIController->SetSensedTarget(aPlayerCharacter);
				AIController->StopMovement();
				MonsterState = EBotBehaviorType::Agression;
				AIController->SetBlackboardBotState(MonsterState);
				AIController->SetLocationVector(Location);
				if (DebugDrawEnabledAI) { DrawDebugSphere(GetWorld(), Location, 10, 5, FColor::Purple, false, 10, 0, 2); }
			}
		}
	}
}


//	Flashed - Sets the AI to Stunned
void AMonster::OnFlashed(APawn* aPawn)
{
	if (GetMonsterDead() == false)
	{
		AMyAIController* AIController = Cast<AMyAIController>(GetController());
		SensedPawn = Cast<ANBCharacter>(aPawn);
		//Set the seen target on the blackboard
		if (AIController && SensedPawn)
		{
			// Is the AI State already Stunned
			if (AIController->IsAIStateStunned()) 
			{
				if (DebugDrawEnabledAI) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, this->GetName() + TEXT(" - AI already stunned!")); }
			}
			else
			{
				AIController->SetAIStateStunned();
				if (DebugDrawEnabledAI) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, this->GetName() + TEXT(" - AI stunned!")); }
				MonsterState = EBotBehaviorType::Stunned;
				AIController->SetBlackboardBotState(MonsterState);
				AIController->SetLocationVector(SensedPawn->GetActorLocation());
				AIController->StopMovement();

				//Material Path
				FString matPath = "Material'/Game/Textures/Monster/Mon_M.Mon_M'";
				//Material Instance
				UMaterialInstanceConstant* material = Cast<UMaterialInstanceConstant>(StaticLoadObject(UMaterialInstanceConstant::StaticClass(), nullptr, *(matPath)));
				this->GetMesh()->SetMaterial(0, material);

				UCapsuleComponent*	CapCom;
				CapCom = this->GetCapsuleComponent();
				FCollisionResponseContainer CollCont = this->GetMesh()->GetCollisionResponseToChannels();

				// Now that we have flashed the Skinny we are allowed to do weapon hits 

//				if (CapCom->GetCollisionResponseToChannels().GetResponse(WEAPON_TRACE) == ECollisionResponse::ECR_Ignore)
				{
					this->GetMesh()->SetCollisionResponseToChannel(WEAPON_TRACE, ECollisionResponse::ECR_Block);
				}

			}
		}
	}
}


//	Shot - Sets the AI to Agression if hit
void AMonster::OnShot(APawn* aPawn)
{
	if (GetMonsterDead() == false)
	{
		AMyAIController* AIController = Cast<AMyAIController>(GetController());
		SensedPawn = Cast<ANBCharacter>(aPawn);
		//Set the seen target on the blackboard
		if (AIController && SensedPawn)
		{
			AIController->SetAIStateFlee();
			if (DebugDrawEnabledAI) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, this->GetName() + TEXT(" - hit - Agression!")); }
			MonsterState = EBotBehaviorType::Agression;
			AIController->SetBlackboardBotState(MonsterState);
			AIController->SetLocationVector(SensedPawn->GetActorLocation());
			AIController->StopMovement();
		}
	}
}


//	Seen - Updates the AI state if seen - GetDistanceTo(SensedPawn) < MaxVisibleRange)
void AMonster::OnSeePlayer(APawn* aPawn)
{
	if (GetMonsterDead() == false)
	{
		EBotBehaviorType PreMonsterState;
		PreMonsterState = MonsterState;

		AMyAIController* AIController = Cast<AMyAIController>(GetController());
		SensedPawn = Cast<ANBCharacter>(aPawn);
		//Set the seen target on the blackboard
		if (AIController && SensedPawn )
		{
			if ((GetDistanceTo(SensedPawn) < MaxVisibleRange))
			{
				// In seeing range 
				// In seeing range 
				// In seeing range 

				switch (MonsterState)
				{
				case EBotBehaviorType::Neutral:
				case EBotBehaviorType::Suspicious:
					if (DebugDrawEnabledAI) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, this->GetName() + TEXT(" - saw me!")); }
					//GLog->Log("Seen");
					MonsterState = EBotBehaviorType::Agression;
					AIController->SetBlackboardBotState(MonsterState);
					AIController->SetLocationVector(SensedPawn->GetActorLocation());
					if (DebugDrawEnabledAI) { DrawDebugSphere(GetWorld(), SensedPawn->GetActorLocation(), 10, 5, FColor::Red, false, 10, 0, 2); }

					AIController->StopMovement();
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
					break;

				case EBotBehaviorType::Agression:
					// AI is already Agressive - just update the location
					AIController->SetLocationVector(SensedPawn->GetActorLocation());
					if (DebugDrawEnabledAI) { DrawDebugSphere(GetWorld(), SensedPawn->GetActorLocation(), 10, 5, FColor::Yellow, false, 10, 0, 2); }
					break;
				case EBotBehaviorType::Charge:
					// Not used
					break;
				case EBotBehaviorType::Stunned:
					break;
				case EBotBehaviorType::Flee:
					break;
				default:
					break;
				}

			}
			else
			{
				GLog->Log("Detected but Out of Seeing range");

				// Detected but Not in seeing raange
				// Detected but Not in seeing raange
				// Detected but Not in seeing raange

				if (SoundIdle)
				{
					AudioLoopComp->SetSound(SoundIdle);
					AudioLoopComp->Play();
					LastIdlePlayTime = GetWorld()->GetTimeSeconds();
					SetPlayModeState(EGameModeSoundType::General);
				}
				AIController->ResetSeenTarget();
			}
		}
	}
}


void AMonster::ReduceHealth(int DamageValue)
{
	if (MonsterState == EBotBehaviorType::Stunned || bisMonsterInLight)
	{
		DamageHealth(DamageValue);
	}

}

void AMonster::DamageHealth(int DamageValue)
{
	if ((Health <= 0) && !bisMonsterDead)
	{
		bisMonsterDead = true;
		SetRagdollPhysics();

		GLog->Log("SetLifeSpan(AfterDeathAutoDelete)");
		SetLifeSpan(AfterDeathAutoDelete);
		SpawnDropItems();

		// Once the AI is dead - turn off Navigation on the object so everyone can walk through the corpse

		UCapsuleComponent*	CapCom;
		CapCom = this->GetCapsuleComponent();
		ECollisionChannel Chan;
		Chan = CapCom->GetCollisionObjectType();
		//CapCom->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
		FCollisionResponseContainer CollCont = CapCom->GetCollisionResponseToChannels();

		if (CapCom->GetCollisionResponseToChannels().GetResponse(ECollisionChannel::ECC_Pawn) == ECollisionResponse::ECR_Block)
		{
			//GLog->Log("Before setting : " + this->GetName() + "ECollisionChannel::ECC_Pawn  ECR_Block ");
			// Assuming the Skinny ECollisionChannel is set to Pawn - once the AI is dea we need to stop the capsule component from blocking movement

			CapCom->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
			
		}



		if (bisScoreAdded == false)
		{
			SensedPawn->IncreaseScore(MonsterValue);
			bisScoreAdded = true;
		}
	}
	else
	{
		Health = Health - DamageValue;
	}
}

bool AMonster::GetMonsterDead()
{
	if (bisMonsterDead)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "GetMonsterDead " + GetName() + " : Health=" + FString::SanitizeFloat(Health));
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

	//if (!bInRagdoll)
	//{
	//	// Immediately hide the pawn
	//	TurnOff();
	//	SetActorHiddenInGame(true);
	//	SetLifeSpan(1.0f);
	//}
	//else
	//{
	//	GLog->Log("SetLifeSpan(10.0f)");
	//	SetLifeSpan(10.0f);
	//}
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
		AudioLoopComp->Stop();
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

void AMonster::SetMonsterDebugDrawAI(bool deb)
{
	DebugDrawEnabledAI = deb;
}

void AMonster::SetMonsterDebugDrawAI2(bool deb)
{
	DebugDrawEnabledAI2 = deb;
}