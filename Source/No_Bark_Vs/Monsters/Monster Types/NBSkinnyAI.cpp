// Fill out your copyright notice in the Description page of Project Settings.

#include "NBSkinnyAI.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Player/NBCharacter.h"
#include "Public/TimerManager.h"
#include "Materials/MaterialInstanceConstant.h"
#include "../AI/NBAIController.h"
#include "Perception/PawnSensingComponent.h"


// Sets default values
ANBSkinnyAI::ANBSkinnyAI()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RightHandSocket = TEXT("rightHandSocket");
	LeftHandSocket = TEXT("leftHandSocket");
	HeadSocket = TEXT("headSocket");
	IsAnimPlaying = false;

	// This sphere component is attached to the hand to detect a hit
	RightHandStrikePlayerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RightHandStrikePlayerSphere"));
	RightHandStrikePlayerSphere->SetSphereRadius(15);
	RightHandStrikePlayerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	RightHandStrikePlayerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RightHandStrikePlayerSphere->SetupAttachment(GetCapsuleComponent());
	RightHandStrikePlayerSphere->OnComponentBeginOverlap.AddDynamic(this, &ANBSkinnyAI::OnOverlapStrikeCharacter);
	RightHandStrikePlayerSphere->OnComponentEndOverlap.AddDynamic(this, &ANBSkinnyAI::OnEndOverlapStrikeCharacter);

	// This sphere component is attached to the hand to detect a hit
	LeftHandStrikePlayerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHandStrikePlayerSphere"));
	LeftHandStrikePlayerSphere->SetSphereRadius(15);
	LeftHandStrikePlayerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	LeftHandStrikePlayerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	LeftHandStrikePlayerSphere->SetupAttachment(GetCapsuleComponent());
	LeftHandStrikePlayerSphere->OnComponentBeginOverlap.AddDynamic(this, &ANBSkinnyAI::OnOverlapStrikeCharacter);
	LeftHandStrikePlayerSphere->OnComponentEndOverlap.AddDynamic(this, &ANBSkinnyAI::OnEndOverlapStrikeCharacter);

	// This sphere component is attached to the hand to detect a hit
	HeadStrikePlayerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("HeadStrikePlayerSphere"));
	HeadStrikePlayerSphere->SetSphereRadius(15);
	HeadStrikePlayerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	HeadStrikePlayerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HeadStrikePlayerSphere->SetupAttachment(GetCapsuleComponent());
	HeadStrikePlayerSphere->OnComponentBeginOverlap.AddDynamic(this, &ANBSkinnyAI::OnOverlapStrikeCharacter);
	HeadStrikePlayerSphere->OnComponentEndOverlap.AddDynamic(this, &ANBSkinnyAI::OnEndOverlapStrikeCharacter);


	// This sphere component surrounds the monster to determine attack range
	AttackRangeAnimationTriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeAnimationTriggerSphere"));
	AttackRangeAnimationTriggerSphere->SetSphereRadius(150);
	AttackRangeAnimationTriggerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackRangeAnimationTriggerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackRangeAnimationTriggerSphere->SetupAttachment(GetCapsuleComponent());
	AttackRangeAnimationTriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ANBSkinnyAI::OnOverlapStartAnim);
	AttackRangeAnimationTriggerSphere->OnComponentEndOverlap.AddDynamic(this, &ANBSkinnyAI::OnEndOverlapStopAnim);

	/* Our sensing component to detect players by visibility and noise checks. */
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->HearingThreshold = 200;
	PawnSensingComp->LOSHearingThreshold = 300;

	NeutralWalkSpeed = 100;
	SuspiciousWalkSpeed = 100;
	AggressionWalkSpeed = 100;
	ChargeWalkSpeed = 100;
	StunnedWalkSpeed = 0;
	FleeWalkSpeed = 100;
	
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
	if (IsAnimPlaying != true)
	{
		PlayAnimation(StunAnimation);
	}
}

void ANBSkinnyAI::OnSeePlayer(APawn * Pawn)
{
	ANBAIController* AIController = Cast<ANBAIController>(GetController());
	ANBCharacter* NBCharacterPawn = Cast<ANBCharacter>(Pawn);
	/* if sensed pawn is the player*/
	if (NBCharacterPawn && AIController)
	{
		AIController->SetLastDetectedLocationKey(NBCharacterPawn->GetActorLocation());
		/*if the monster detect the player for first  time*/
		if (bIsSuspicious == false)
		{
			FirstDetectedTime = GetWorld()->GetTimeSeconds();
			bIsSuspicious = true;
			SetAIState(EBotBehaviorType::Suspicious);
		}
		/*When the monster has already seen you few seconds ago*/
		else
		{
			//continue updating Last SeenTime.
			LastDetectedTime = GetWorld()->GetTimeSeconds();


			//if the last time seen is bigger than maximun duration
			if (LastDetectedTime - FirstDetectedTime > NBCharacterPawn->ValToMakePawnUnDetected* DetectionMaxTime)
			{
			//	SetAIState(EBotBehaviorType::Agression);
			}
		}

		
	}

	
}

void ANBSkinnyAI::OnHearNoise(APawn * PawnInstigator, const FVector & Location, float Volume)
{
	FString TheFloatStr = FString::SanitizeFloat(Volume);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Hearing Noise!"+ TheFloatStr);
}

void ANBSkinnyAI::SimulateMeleeStrike()
{
	EAttackValue AttackType = static_cast<EAttackValue>(FMath::RandRange(0, 2));
	UAnimMontage* AttackAnimation = GetAttackAnim(AttackType);
	if (IsAnimPlaying != true)
	{
		PlayAnimation(AttackAnimation);
		//PlaySound(SoundIdle);
	}

}


void ANBSkinnyAI::OnOverlapStrikeCharacter(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		ANBCharacter* OtherPawn = Cast<ANBCharacter>(OtherActor);
		if (OtherPawn)
		{
			//health decrease of other pawn. 
			
		}
	}
}

void ANBSkinnyAI::OnEndOverlapStrikeCharacter(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{

	}
}

void ANBSkinnyAI::OnOverlapStartAnim(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		//Settimeto start for animation and sound of melleestrike. 
		GetWorldTimerManager().SetTimer(TimerHandle_MeleeAttack, this, &ANBSkinnyAI::SimulateMeleeStrike, 2.0, true, 0.0f);
	}
	
}

void ANBSkinnyAI::OnEndOverlapStopAnim(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		//Stops Stamina increase
		GetWorldTimerManager().ClearTimer(TimerHandle_MeleeAttack);
	}
}

void ANBSkinnyAI::SetTranparentMaterial()
{
	//Material Path
	FString matPath = "Material'/Game/Textures/Monster/Mon_M_Darkness.uasset'";
	//Material Instance
	UMaterialInstanceConstant* material = Cast<UMaterialInstanceConstant>(StaticLoadObject(UMaterialInstanceConstant::StaticClass(), nullptr, *(matPath)));
	this->GetMesh()->SetMaterial(0, material);
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
		switch (AIState)
		{
		case EBotBehaviorType::Neutral:
			AIController->SetAIStateKey(EBotBehaviorType::Neutral);
			SetWalkSpeed(NeutralWalkSpeed);
			break;
		case EBotBehaviorType::Suspicious:
			AIController->SetAIStateKey(EBotBehaviorType::Suspicious);
			SetWalkSpeed(SuspiciousWalkSpeed);
			break;
		case EBotBehaviorType::Agression:
			AIController->SetAIStateKey(EBotBehaviorType::Agression);
			SetWalkSpeed(AggressionWalkSpeed);
			break;
		case EBotBehaviorType::Charge:
			AIController->SetAIStateKey(EBotBehaviorType::Charge);
			SetWalkSpeed(ChargeWalkSpeed);
			break;
		case EBotBehaviorType::Stunned:
			AIController->SetAIStateKey(EBotBehaviorType::Stunned);
			SetWalkSpeed(StunnedWalkSpeed);
			break;
		case EBotBehaviorType::Flee:
			AIController->SetAIStateKey(EBotBehaviorType::Flee);
			SetWalkSpeed(FleeWalkSpeed);
			break;
		default:
			break;
		}	// passive patrolling mode
	}
	
	
}
