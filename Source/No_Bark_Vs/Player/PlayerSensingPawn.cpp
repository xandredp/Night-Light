// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSensingPawn.h"
#include "Player/PlayController.h"
#include "Perception/PawnSensingComponent.h"

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

	//PlayPickupAnimation
	ANBCharacter* MyPawn = Cast<ANBCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (MyPawn)
	{
		if (MyPawn->SensingComponentPawn == nullptr)
		{
			MyPawn->SensingComponentPawn = this;
		}
	}

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &APlayerSensingPawn::OnSeeEnemy);
		//	PawnSensingComp->OnHearNoise.AddDynamic(this, &ANBCharacter::OnHearNoise);
	}
}
void APlayerSensingPawn::OnSeeEnemy(APawn * Pawn)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, Pawn->GetName());
}