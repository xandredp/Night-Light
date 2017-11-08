// Fill out your copyright notice in the Description page of Project Settings.

#include "NBSkinnyAI.h"
#include "Core/No_Bark_Vs.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Player/NBCharacter.h"
#include "Public/TimerManager.h"
#include "Materials/MaterialInstanceConstant.h"
#include "../AI/NBAIController.h"
#include "Perception/PawnSensingComponent.h"
#include "../../Player/PlayerSensingPawn.h"

// Sets default values
ANBSkinnyAI::ANBSkinnyAI(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	RightHandSocket = TEXT("rightHandSocket");
	LeftHandSocket = TEXT("leftHandSocket");
	HeadSocket = TEXT("headSocket");
	IsAnimPlaying = false;

	// This sphere component is attached to the hand to detect a hit
	RightHandStrikePlayerSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("RightHandStrikePlayerSphere"));
	RightHandStrikePlayerSphere->SetSphereRadius(10);
	RightHandStrikePlayerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	RightHandStrikePlayerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RightHandStrikePlayerSphere->SetupAttachment(GetCapsuleComponent());
	RightHandStrikePlayerSphere->OnComponentBeginOverlap.AddDynamic(this, &ANBSkinnyAI::OnOverlapStrikeCharacter);
	RightHandStrikePlayerSphere->OnComponentEndOverlap.AddDynamic(this, &ANBSkinnyAI::OnEndOverlapStrikeCharacter);

	// This sphere component is attached to the hand to detect a hit
	LeftHandStrikePlayerSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("LeftHandStrikePlayerSphere"));
	LeftHandStrikePlayerSphere->SetSphereRadius(10);
	LeftHandStrikePlayerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeftHandStrikePlayerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	LeftHandStrikePlayerSphere->SetupAttachment(GetCapsuleComponent());
	LeftHandStrikePlayerSphere->OnComponentBeginOverlap.AddDynamic(this, &ANBSkinnyAI::OnOverlapStrikeCharacter);
	LeftHandStrikePlayerSphere->OnComponentEndOverlap.AddDynamic(this, &ANBSkinnyAI::OnEndOverlapStrikeCharacter);

	// This sphere component is attached to the hand to detect a hit
	HeadStrikePlayerSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("HeadStrikePlayerSphere"));
	HeadStrikePlayerSphere->SetSphereRadius(12);
	HeadStrikePlayerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	HeadStrikePlayerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HeadStrikePlayerSphere->SetupAttachment(GetCapsuleComponent());
	HeadStrikePlayerSphere->OnComponentBeginOverlap.AddDynamic(this, &ANBSkinnyAI::OnOverlapStrikeCharacter);
	HeadStrikePlayerSphere->OnComponentEndOverlap.AddDynamic(this, &ANBSkinnyAI::OnEndOverlapStrikeCharacter);


	// This sphere component surrounds the monster to determine attack range
	AttackRangeAnimationTriggerSphere = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("AttackRangeAnimationTriggerSphere"));
	AttackRangeAnimationTriggerSphere->SetSphereRadius(150);
	AttackRangeAnimationTriggerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackRangeAnimationTriggerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackRangeAnimationTriggerSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	AttackRangeAnimationTriggerSphere->SetupAttachment(GetCapsuleComponent());
	AttackRangeAnimationTriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ANBSkinnyAI::OnOverlapStartAnim);
	AttackRangeAnimationTriggerSphere->OnComponentEndOverlap.AddDynamic(this, &ANBSkinnyAI::OnEndOverlapStopAnim);

	/* Our sensing component to detect players by visibility and noise checks. */
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->HearingThreshold = 200;
	PawnSensingComp->LOSHearingThreshold = 1000;

	NeutralWalkSpeed = 100;
	SuspiciousWalkSpeed = 150;
	AggressionWalkSpeed = 200;
	ChargeWalkSpeed = 100;
	StunnedWalkSpeed = 0;
	FleeWalkSpeed = 100;
	SenseTimeOut = 10.0;
	AgrresionTimeOut = 3.0;
	DetectionMaxTime = 10.0f;
	//BaseMonster Variables
	CurrentAttackDamage = 20.0f;
	InLightAttackDamage = 10.0f;
	InDarkAttackDamage = 30.0f;
	InLightMovementSpeed = 0.8f;
	InDarkMovementSpeed = 1.5f;
	MaxHealth = 100.0f;
	Health = MaxHealth;
	MinAgressiveDistant = 1000;
	IsEnemyInCircle=false
	

}

// Called when the game starts or when spawned
void ANBSkinnyAI::BeginPlay()
{
	Super::BeginPlay();
	RightHandStrikePlayerSphere->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, RightHandSocket);
	LeftHandStrikePlayerSphere->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, LeftHandSocket);
	HeadStrikePlayerSphere->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, HeadSocket);

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &ANBSkinnyAI::OnSeePlayer);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &ANBSkinnyAI::OnHearNoise);
	}

	SetAIState(EBotBehaviorType::Neutral);

}

void ANBSkinnyAI::OnStun()
{
	SetAIState(EBotBehaviorType::Stunned);
}

void ANBSkinnyAI::OnReact()
{
	if (IsAnimPlaying != true)
	{
		PlayAnimation(ReactAnimation);
	}
}
void ANBSkinnyAI::OnSeePlayer(APawn * Pawn)
{
	if (GetMonsterDead() == false)
	{
		ANBAIController* AIController = Cast<ANBAIController>(GetController());
		ANBCharacter* NBCharacterPawn = Cast<ANBCharacter>(Pawn);

		/* if sensed pawn is the player*/
		if (NBCharacterPawn && AIController)
		{
			/*SetsTargetKey*/
			if (NBPlayerCharacter == nullptr)
			{
				NBPlayerCharacter = NBCharacterPawn;
				AIController->SetTargetKey(NBPlayerCharacter);
			}
			else
			{
				AIController->SetTargetKey(NBPlayerCharacter);
			}

			EBotBehaviorType AIState = AIController->GetAIStateKey();

			if (AIState != EBotBehaviorType::Stunned)
			{
				/*If firt time seen react and go aggressive. */
				if (bIsSuspicious == false)
				{
					FirstDetectedTime = GetWorld()->GetTimeSeconds();
					LastDetectedTime = GetWorld()->GetTimeSeconds();
					bIsSuspicious = true;
					SetAIState(EBotBehaviorType::Suspicious);
					OnReact();
					SetAIState(EBotBehaviorType::Agression);
				} 	/*If seen just go aggressive. */
				else
				{
					//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "SeenAggressive");
					LastDetectedTime = GetWorld()->GetTimeSeconds();
					SetAIState(EBotBehaviorType::Agression);
				}
			}
		}
	}
}

void ANBSkinnyAI::OnHearNoise(APawn * PawnInstigator, const FVector & Location, float Volume)
{
	if (GetMonsterDead() == false)
	{
		ANBAIController* AIController = Cast<ANBAIController>(GetController());
		if (AIController)
		{

			ANBAIController* AIController = Cast<ANBAIController>(GetController());
			ANBCharacter* NBCharacterPawn = Cast<ANBCharacter>(PawnInstigator);
			/* if sensed pawn is the player*/
			if (NBCharacterPawn && AIController)
			{
				EBotBehaviorType AIState = AIController->GetAIStateKey();

				if (AIState != EBotBehaviorType::Stunned)
				{
					if (NBPlayerCharacter == nullptr)
					{
						NBPlayerCharacter = NBCharacterPawn;
						AIController->SetTargetKey(NBPlayerCharacter);
					}
					else
					{
						AIController->SetTargetKey(NBPlayerCharacter);
					}
					if (ASoundBlockingActor* blockingActor = GetSoundBlockingActorInView())
					{
						//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "blocked");
					}
					//if there is nothing blocking in between assign the target enemy
					else
					{
						//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "attack");
						if (bIsSuspicious == false)
						{
							FirstDetectedTime = GetWorld()->GetTimeSeconds();
							LastDetectedTime = GetWorld()->GetTimeSeconds();
							bIsSuspicious = true;
							SetAIState(EBotBehaviorType::Suspicious);
							OnReact();
							SetAIState(EBotBehaviorType::Agression);
						} 	/*If seen just go aggressive. */
						else
						{
							LastDetectedTime = GetWorld()->GetTimeSeconds();
							SetAIState(EBotBehaviorType::Agression);
						}
					}
				}
			}
		}
	}
}

void ANBSkinnyAI::OnShotAt()
{
	if (GetMonsterDead() == false)
	{
		ANBAIController* AIController = Cast<ANBAIController>(GetController());
		if (AIController)
		{
			EBotBehaviorType AIState = AIController->GetAIStateKey();

			if (AIState != EBotBehaviorType::Stunned)
			{
				if (NBPlayerCharacter != nullptr)
				{
					AIController->SetLastDetectedLocationKey(NBPlayerCharacter->GetActorLocation());
				}
				if (bIsSuspicious == false)
				{
					FirstDetectedTime = GetWorld()->GetTimeSeconds();
					LastDetectedTime = GetWorld()->GetTimeSeconds();
					bIsSuspicious = true;
					SetAIState(EBotBehaviorType::Suspicious);
					OnReact();
					SetAIState(EBotBehaviorType::Agression);
				}
				else
				{
					SetAIState(EBotBehaviorType::Agression);
				}
			}
		}
	}
}
void ANBSkinnyAI::OnLostPlayer()
{
	if (IsAnimPlaying != true)
	{
		PlayAnimation(SearchAnimation);
	}
}
void ANBSkinnyAI::CountingPlayerUndetectedTime()
{
	//if player has not been seen bigger amount than the sense time out
	/*Currenttime - last detected time > SenseTime out*/
	if (bIsSuspicious)
	{
		ANBAIController* AIController = Cast<ANBAIController>(GetController());
		if (AIController)
		{
			EBotBehaviorType AIState = AIController->GetAIStateKey();

			if (AIState != EBotBehaviorType::Stunned)
			{
				if (AIState == EBotBehaviorType::Agression)
				{
					if (GetWorld()->TimeSeconds - LastDetectedTime > AgrresionTimeOut)
					{
						//set back the monster to neutral 
						OnLostPlayer();
						SetAIState(EBotBehaviorType::Suspicious);
					}
				}
				else
				{
					if (GetWorld()->TimeSeconds - LastDetectedTime > SenseTimeOut)
					{
						//set back the monster to neutral 
						SetAIState(EBotBehaviorType::Neutral);

					}
				}
			}
			
		}
	}

}
void ANBSkinnyAI::SimulateMeleeStrike()
{
	if (GetMonsterDead() == false)
	{
		EAttackValue AttackType = static_cast<EAttackValue>(FMath::RandRange(0, 2));
		UAnimMontage* AttackAnimation = GetAttackAnim(AttackType);
		if (IsAnimPlaying != true)
		{
			PlayAnimation(AttackAnimation);
			//PlaySound(SoundIdle);
		}

	}
}


void ANBSkinnyAI::OnOverlapStrikeCharacter(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (GetMonsterDead() == false)
	{
		if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
		{
			ANBCharacter* OtherPawn = Cast<ANBCharacter>(OtherActor);
			if (OtherPawn)
			{
				if (OtherPawn->IsBeingAttacked == true)
				{
					//health decrease of other pawn. 
					OtherPawn->DecreaseHealth(CurrentAttackDamage);
				}
				else
				{
					//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "false");
				}
			}
		}
	}
	
}

void ANBSkinnyAI::OnEndOverlapStrikeCharacter(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		ANBCharacter* OtherPawn = Cast<ANBCharacter>(OtherActor);
		if (OtherPawn)
		{
			OtherPawn->IsBeingAttacked = false;
		}
	
	}
}

void ANBSkinnyAI::OnOverlapStartAnim(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (GetMonsterDead() == false)
	{

			ANBAIController* AIController = Cast<ANBAIController>(GetController());
			ANBCharacter* NBCharacterPawn = Cast<ANBCharacter>(OtherActor);

			/* if sensed pawn is the player*/
			if (NBCharacterPawn && AIController)
			{
				IsEnemyInCircle = true;
				EBotBehaviorType AIState = AIController->GetAIStateKey();

				if (AIState != EBotBehaviorType::Stunned)
				{
					AIController->SetLastDetectedLocationKey(NBCharacterPawn->GetActorLocation());

					if (bIsSuspicious == false)
					{
						FirstDetectedTime = GetWorld()->GetTimeSeconds();
						LastDetectedTime = GetWorld()->GetTimeSeconds();
						bIsSuspicious = true;
						SetAIState(EBotBehaviorType::Suspicious);
						OnReact();
						SetAIState(EBotBehaviorType::Agression);
					}
					/*When the monster has already seen you few seconds ago*/
					else
					{
						LastDetectedTime = GetWorld()->GetTimeSeconds();
						SetAIState(EBotBehaviorType::Agression);
					}

					//Settimeto start for animation and sound of melleestrike. 
					GetWorldTimerManager().SetTimer(TimerHandle_MeleeAttack, this, &ANBSkinnyAI::SimulateMeleeStrike, 1.0, true, 0.0f);

				}
							}

		
	}
	
}

void ANBSkinnyAI::OnEndOverlapStopAnim(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{	
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{

		ANBAIController* AIController = Cast<ANBAIController>(GetController());
		ANBCharacter* NBCharacterPawn = Cast<ANBCharacter>(OtherActor);

		/* if sensed pawn is the player*/
		if (NBCharacterPawn && AIController)
		{
			IsEnemyInCircle = false;
			//Stops Stamina increase
			GetWorldTimerManager().ClearTimer(TimerHandle_MeleeAttack);

		}
	}
}

void ANBSkinnyAI::SetTranparentMaterial()
{
	if (GetMonsterDead() == false)
	{
		//Material Path
		FString matPath = "Material'/Game/Textures/Monster/Mon_M_Darkness.uasset'";
		//Material Instance
		UMaterialInstanceConstant* material = Cast<UMaterialInstanceConstant>(StaticLoadObject(UMaterialInstanceConstant::StaticClass(), nullptr, *(matPath)));
		this->GetMesh()->SetMaterial(0, material);
	}
}



UAnimMontage* ANBSkinnyAI::GetAttackAnim(EAttackValue AttackType)
{
	switch (AttackType)
	{
	case LHand:
		return AttackWithRightHandAnimation;

	case RHand:
		return AttackWithLeftHandAnimation;

	case Head:
		return AttackWithHeadAnimation;
	default:
		return nullptr;
	}
}

void ANBSkinnyAI::SetAIState(EBotBehaviorType AIState)
{
	ANBAIController* AIController = Cast<ANBAIController>(GetController());
	if (AIController)
	{
		SetWalkSpeedbyCurrentStatus();
		switch (AIState)
		{
		case EBotBehaviorType::Neutral:
			AIController->SetAIStateKey(EBotBehaviorType::Neutral);
			bIsSuspicious = false;
			//Stops Stamina increase
			GetWorldTimerManager().ClearTimer(TimerHandle_CountUnSeenTime);
			break;
		case EBotBehaviorType::Suspicious:
			AIController->SetAIStateKey(EBotBehaviorType::Suspicious);
			//Settimeto start for animation and sound of melleestrike. 
			GetWorldTimerManager().SetTimer(TimerHandle_CountUnSeenTime, this, &ANBSkinnyAI::CountingPlayerUndetectedTime, 1.0, true, 0.0f);
			break;
		case EBotBehaviorType::Agression:		
			AIController->SetAIStateKey(EBotBehaviorType::Agression);
			break;
		case EBotBehaviorType::Charge:
			AIController->SetAIStateKey(EBotBehaviorType::Charge);
			break;
		case EBotBehaviorType::Stunned:
			AIController->SetAIStateKey(EBotBehaviorType::Stunned);
			if (IsAnimPlaying != true)
			{
				if (bisMonsterkillableAtStun)
				{
					bisMonsterInLight = true;
					MonsterInLight(bisMonsterInLight);
				}
				PlayAnimation(StunAnimation);
			}

			break;
		case EBotBehaviorType::Flee:
			AIController->SetAIStateKey(EBotBehaviorType::Flee);
			break;
		default:
			break;
		}	// passive patrolling mode
	}
	
	
}

void ANBSkinnyAI::SetWalkSpeedbyCurrentStatus()
{
	ANBAIController* AIController = Cast<ANBAIController>(GetController());
	if (AIController)
	{
		EBotBehaviorType AIState = AIController->GetAIStateKey();

		switch (AIState)
		{
		case EBotBehaviorType::Neutral:
			SetWalkSpeed(NeutralWalkSpeed);
			break;
		case EBotBehaviorType::Suspicious:
			SetWalkSpeed(SuspiciousWalkSpeed);
			break;
		case EBotBehaviorType::Agression:
			SetWalkSpeed(AggressionWalkSpeed);
			break;
		case EBotBehaviorType::Charge:
			SetWalkSpeed(ChargeWalkSpeed);
			break;
		case EBotBehaviorType::Stunned:
			SetWalkSpeed(StunnedWalkSpeed);
			break;
		case EBotBehaviorType::Flee:
			SetWalkSpeed(FleeWalkSpeed);
			break;
		default:
			break;
		}	// passive patrolling mode
	}
	
	
}
