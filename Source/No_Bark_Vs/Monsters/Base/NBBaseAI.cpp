// Fill out your copyright notice in the Description page of Project Settings.

#include "NBBaseAI.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
//#include "Engine/EngineTypes.h"


// Sets default values
ANBBaseAI::ANBBaseAI()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

void ANBBaseAI::SetRagdollPhysics()
{
	USkeletalMeshComponent* Mesh3P = GetMesh();

	const FName& boneName = "skeleton";
		Mesh3P->SetAllBodiesBelowSimulatePhysics(boneName, true, true);
		Mesh3P->SetAllBodiesBelowPhysicsBlendWeight(boneName, 1.0, false, true);

	UCharacterMovementComponent* CharacterComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
	if (CharacterComp)
	{
		CharacterComp->StopMovementImmediately();
		CharacterComp->DisableMovement();
		CharacterComp->SetComponentTickEnabled(false);
	}

}

void ANBBaseAI::EnableCollisionCapsuleCollision(bool bEnableCollision)
{
	if (bEnableCollision == true)
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}
