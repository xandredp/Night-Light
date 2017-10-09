// Fill out your copyright notice in the Description page of Project Settings.

#include "NBBaseAI.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ANBBaseAI::ANBBaseAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioLoopComp = CreateDefaultSubobject<UAudioComponent>(TEXT("MonsterLoopedSoundComp"));
	AudioLoopComp->bAutoActivate = false;
	AudioLoopComp->bAutoDestroy = false;
	AudioLoopComp->SetupAttachment(RootComponent);

	/* Our sensing component to detect players by visibility and noise checks. */
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->HearingThreshold = 600;
	PawnSensingComp->LOSHearingThreshold = 1200;

	bIsSuspicious = false;


}

// Called when the game starts or when spawned
void ANBBaseAI::BeginPlay()
{
	Super::BeginPlay();
	
	/*To give smooth rotation*/
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

}

// Called every frame
void ANBBaseAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//OnDeath function sets ragdoll and stops the sound. 
void ANBBaseAI::OnDeath()
{
	if (Health <= 0.0f)
	{	//play sound
		//AudioLoopComp->SetSound(SoundDeathAttack);
		AudioLoopComp->Play();

		DetachFromControllerPendingDestroy();
		/* Disable all collision on capsule */
		UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

		USkeletalMeshComponent* Mesh3P = GetMesh();
		if (Mesh3P)
		{
			Mesh3P->SetCollisionProfileName(TEXT("Ragdoll"));
		}
		SetActorEnableCollision(true);

		SetRagdollPhysics("skeleton", 1.0, false, false, true);

		//stop sound
		AudioLoopComp->Stop();
	}
}
void ANBBaseAI::OnStun()
{
}
void ANBBaseAI::OnReact()
{
}
void ANBBaseAI::OnSeePlayer(APawn * Pawn)
{
}
void ANBBaseAI::OnHearNoise(APawn * PawnInstigator, const FVector & Location, float Volume)
{
}
void ANBBaseAI::ReduceHealth(int DamageValue)
{
	if (Health <= 0.0f)
	{
		OnDeath();
	}
	else
	{
		Health -= DamageValue;
	}
}

void ANBBaseAI::SetRagdollPhysics(const FName & boneName, float PhysicsBlendWeight, bool bNewSimulate, bool bRecover, bool bMeshDead)
{
	USkeletalMeshComponent* Mesh3P = GetMesh();
	if (bMeshDead == true)
	{

		Mesh3P->SetAllBodiesSimulatePhysics(true);
		Mesh3P->SetSimulatePhysics(true);
		Mesh3P->WakeAllRigidBodies();
		Mesh3P->bBlendPhysics = true;

		UCharacterMovementComponent* CharacterComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
		if (CharacterComp)
		{
			CharacterComp->StopMovementImmediately();
			CharacterComp->DisableMovement();
			CharacterComp->SetComponentTickEnabled(false);
		}
		SetLifeSpan(10.0f);
	}
	else
	{
		if (bRecover == true)
		{
			Mesh3P->SetAllBodiesBelowSimulatePhysics(boneName, bNewSimulate, true);
			Mesh3P->SetAllBodiesBelowPhysicsBlendWeight(boneName, PhysicsBlendWeight, false, true);
		}
		if (bRecover == false)
		{
			Mesh3P->SetAllBodiesBelowPhysicsBlendWeight(boneName, PhysicsBlendWeight, false, true);
			Mesh3P->SetAllBodiesBelowSimulatePhysics(boneName, bNewSimulate, true);
		}
	}
}
void ANBBaseAI::SetWalkSpeed(float desiredWalkSpeed)
{
	GetCharacterMovement()->MaxWalkSpeed = desiredWalkSpeed;
}

void ANBBaseAI::PlaySound(class USoundCue * SoundToPlay)
{
	if (SoundToPlay)
	{
		AudioLoopComp->SetSound(SoundToPlay);
		AudioLoopComp->Play();
	}
}

void ANBBaseAI::PlayAnimation(UAnimMontage * AnimMontageToPlay)
{
	PlayAnimMontage(AnimMontageToPlay);
}

