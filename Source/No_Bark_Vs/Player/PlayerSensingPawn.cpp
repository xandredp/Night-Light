// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSensingPawn.h"
#include "Player/PlayController.h"
#include "Perception/PawnSensingComponent.h"
#include "../Monsters/Monster Types/NBSkinnyAI.h"

// Sets default values
APlayerSensingPawn::APlayerSensingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	/* Our sensing component to detect enemies by visibility and noise checks. */
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(33.0f);
	PawnSensingComp->SightRadius = 2000;
	PawnSensingComp->HearingThreshold = 600;
	PawnSensingComp->LOSHearingThreshold = 1200;
	PawnSensingComp->bOnlySensePlayers = false;

}

// Called when the game starts or when spawned
void APlayerSensingPawn::BeginPlay()
{
	Super::BeginPlay();

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &APlayerSensingPawn::OnSeeEnemy);
		//	PawnSensingComp->OnHearNoise.AddDynamic(this, &ANBCharacter::OnHearNoise);
	}
	//Set owning player. and assign sensing component pawn. When Assigned this will be attached to gun when torch is attached.
	ANBCharacter* OwnerPawn = Cast<ANBCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (OwnerPawn)
	{
		NBCharacter = OwnerPawn;
		if (OwnerPawn->SensingComponentPawn == nullptr)
		{
			OwnerPawn->SensingComponentPawn = this;
		}
	}


	
}
void APlayerSensingPawn::OnSeeEnemy(APawn * Pawn)
{
	//check if torch is not null
	if (NBCharacter->CurrentTorch)
	{	//when torch is activated

		ANBSkinnyAI *Enemy = Cast<ANBSkinnyAI>(Pawn);
		if (Enemy)
		{
			NBCharacter->CurrentTorch->IsEnemySeen = true;
			NBCharacter->CurrentTorch->EnemyPawn = Enemy;
			//SpawnParticleEffect(Pawn);
		}
	}
}
