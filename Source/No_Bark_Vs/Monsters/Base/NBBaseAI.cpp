// Fill out your copyright notice in the Description page of Project Settings.

#include "NBBaseAI.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
//#include "Components/CapsuleComponent.h"


// Sets default values
ANBBaseAI::ANBBaseAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioLoopComp = CreateDefaultSubobject<UAudioComponent>(TEXT("MonsterLoopedSoundComp"));
	AudioLoopComp->bAutoActivate = false;
	AudioLoopComp->bAutoDestroy = false;
	AudioLoopComp->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ANBBaseAI::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANBBaseAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ANBBaseAI::SetRagdollPhysics(const FName & boneName, float PhysicsBlendWeight, bool bNewSimulate, bool bRecover, bool bMeshDead)
{
	USkeletalMeshComponent* Mesh3P = GetMesh();

	if (bRecover == true)
	{
		Mesh3P->SetAllBodiesBelowSimulatePhysics(boneName, bNewSimulate, true);
		Mesh3P->SetAllBodiesBelowPhysicsBlendWeight(boneName, PhysicsBlendWeight, false, true);
	}
	else
	{
		Mesh3P->SetAllBodiesBelowPhysicsBlendWeight(boneName, PhysicsBlendWeight, false, true);
		Mesh3P->SetAllBodiesBelowSimulatePhysics(boneName, bNewSimulate, true);
	
		if (bMeshDead == true)
		{
			UCharacterMovementComponent* CharacterComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
			if (CharacterComp)
			{
				CharacterComp->StopMovementImmediately();
				CharacterComp->DisableMovement();
				CharacterComp->SetComponentTickEnabled(false);
			}
		}
	}
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

