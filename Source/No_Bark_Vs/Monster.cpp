// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "Monster.h"
#include "MyAIController.h"
#include "TypeClass.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AMonster::AMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Initializing the pawn sensing component
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	// Set the peripheral vision angle to 80 degrees
	PawnSensingComp->SetPeripheralVisionAngle(80.0f);

	MonsterState = EBotBehaviorType::Neutral;



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
void AMonster::OnSeePlayer(APawn* Pawn)
{
	AMyAIController* AIController = Cast<AMyAIController>(GetController());

	//Set the seen target on the blackboard
	if (AIController)
	{
		GLog->Log("Seen");
		AIController->SetSeenTarget(Pawn);
	}
}


// Called every frame
void AMonster::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AMonster::TakeDamage()
{
}
