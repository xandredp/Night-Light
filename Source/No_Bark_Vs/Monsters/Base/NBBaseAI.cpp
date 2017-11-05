// Fill out your copyright notice in the Description page of Project Settings.

#include "NBBaseAI.h"
#include "Core/No_Bark_Vs.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundCue.h"
#include "Components/CapsuleComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ANBBaseAI::ANBBaseAI(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AudioLoopComp = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("MonsterLoopedSoundComp"));
	AudioLoopComp->bAutoActivate = false;
	AudioLoopComp->bAutoDestroy = false;
	AudioLoopComp->SetupAttachment(RootComponent);

	/* Our sensing component to detect players by visibility and noise checks. */
	PawnSensingComp = ObjectInitializer.CreateDefaultSubobject<UPawnSensingComponent>(this, TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(60.0f);
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->HearingThreshold = 600;
	PawnSensingComp->LOSHearingThreshold = 1200;

	bIsSuspicious = false;
	
	MaxHealth = 100.0f;
	Health = MaxHealth;
	bisMonsterkillableAtStun = false;

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

ASoundBlockingActor * ANBBaseAI::GetSoundBlockingActorInView()
{
	FCollisionQueryParams TraceParams(TEXT("TraceBlcokingActor"), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = false;

	/* Not tracing complex uses the rough collision instead making tiny objects easier to select. */
	TraceParams.bTraceComplex = false;

	ANBAIController* AIController = Cast<ANBAIController>(GetController());
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	// Re-Initialize hit info
	FHitResult Hit(ForceInit);
	// Define start point of the trace
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	APawn* AIPawn = AIController->GetPawn();
	FVector AILocation = AIPawn->GetActorLocation();

	//	Controller->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceStart = PlayerLocation;
	const FVector TraceEnd = AILocation;
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, TraceParams);
	//DrawDebugLine(GetWorld(), TraceStart, Hit.TraceEnd, FColor::Green, true, 0.05f, 0.0f, 1.0f);
	return Cast<ASoundBlockingActor>(Hit.GetActor());
}

//OnDeath function sets ragdoll and stops the sound. 
void ANBBaseAI::OnDeath()
{
	if (Health <= 0.0f)
	{	//play sound
		//AudioLoopComp->SetSound(SoundDeathAttack);
		AudioLoopComp->Play();
		SpawnDropItems();
		
		DetachFromControllerPendingDestroy();
		/* Disable all collision on capsule */
		//UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
		//GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		//GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

		USkeletalMeshComponent* Mesh3P = GetMesh();
		if (Mesh3P)
		{
			Mesh3P->SetCollisionProfileName(TEXT("Ragdoll"));
		
		}
		//stop sound
		AudioLoopComp->Stop();

		UCharacterMovementComponent* CharacterComp = Cast<UCharacterMovementComponent>(GetMovementComponent());
		if (CharacterComp)
		{
			CharacterComp->StopMovementImmediately();
			CharacterComp->DisableMovement();
			CharacterComp->SetComponentTickEnabled(false);
		}
		SetLifeSpan(10.0f);
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
void ANBBaseAI::OnShotAt()
{
}
bool ANBBaseAI::GetMonsterDead()
{
	if (Health <= 0.0f)
	{
		OnDeath();
		return true;
	}
	else 
	{
		return false;
	}

}
void ANBBaseAI::ApplyDamage(AActor * DamagedActor, float BaseDamage, FVector const & HitFromDirection, FHitResult const & HitInfo, AController * EventInstigator, AActor * DamageCauser, TSubclassOf<UDamageType> DamageTypeClass)
{
	UGameplayStatics::ApplyPointDamage(DamagedActor, BaseDamage, HitFromDirection, HitInfo, EventInstigator, DamageCauser, DamageTypeClass);
}

void ANBBaseAI::ReduceHealth(int DamageValue)
{
	if (Health <= 0.0f)
	{
		Health = 0;
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
	if (bisMonsterInLight)
	{
		GetCharacterMovement()->MaxWalkSpeed = desiredWalkSpeed * InLightMovementSpeed ;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = desiredWalkSpeed * InDarkMovementSpeed;
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

