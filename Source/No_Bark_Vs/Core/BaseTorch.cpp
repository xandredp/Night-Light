// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseTorch.h"
#include "Core/No_Bark_Vs.h"
#include "Player/NBCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SpotLightComponent.h"



// Sets default values
ABaseTorch::ABaseTorch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TorchSpotlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("WeaponSpotlight"));
	TorchSpotlight->SetupAttachment(RootComponent);
	TorchSpotlight->SetVisibility(true);
	TorchSpotlight->SetRelativeRotation(FRotator(90, 0, 0));
	TorchSpotlight->SetRelativeLocation(FVector(0, 0, -0));

	TorchSpotlight->SetIntensity(8000);
	TorchSpotlight->SetAttenuationRadius(16000);
	TorchSpotlight->SetOuterConeAngle(10);




	MaxEnergy = 100.0f;
	CurrentEnergy = MaxEnergy;
	MaxUseDistance = 600.0f;
	CurrentUseDistance = MaxUseDistance;
	EnergyReductionRate = 1.0f;
	ReductionTimerRate = 0.5f;
	EnergyReductionOnPowerUse = 20.0f;
	EnergyIncreaseOnCrank = 5.0;


}

// Called when the game starts or when spawned
void ABaseTorch::BeginPlay()
{
	Super::BeginPlay();

	SetTorchIntensity();
	
}

// Called every frame
//void ABaseTorch::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}


void ABaseTorch::SetOwningPawn(class ANBCharacter* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}
void ABaseTorch::SetTorchIntensity()
{
	float EnergyRatio = CurrentEnergy / 100;
	if (EnergyRatio > 0.9)
	{
		TorchSpotlight->SetIntensity(8000);
	}
	else
	{
		TorchSpotlight->SetIntensity(4000 * EnergyRatio);
	}
	CurrentUseDistance = MaxUseDistance * EnergyRatio;
}

void ABaseTorch::DrainTorchEnergy()
{
	if (CurrentEnergy <= 0)
	{
		GetWorldTimerManager().ClearTimer(StartReducingEnergyTimerHandle);
		CurrentEnergy = 0;
	}
	else
	{
		CurrentEnergy = CurrentEnergy - EnergyReductionRate;
	}
	SetTorchIntensity();
}

void ABaseTorch::DecreaseEnergy()
{
	if (CurrentEnergy - EnergyReductionOnPowerUse >= 0)
	{
		CurrentEnergy = CurrentEnergy - EnergyReductionOnPowerUse;
	}
	SetTorchIntensity();
}

void ABaseTorch::IncreaseEnergy()
{
	CurrentEnergy = CurrentEnergy + EnergyIncreaseOnCrank;
	if (CurrentEnergy >= MaxEnergy)
	{
		CurrentEnergy = MaxEnergy;
	}
	SetTorchIntensity();
}



void ABaseTorch::ActivateTorch()
{
	//if we have enough power
	if (CurrentEnergy - EnergyReductionOnPowerUse >= 0)
	{
		// 1. decrase energy
		DecreaseEnergy();
		// 2. StunEnemy
		// 3. blueprintimplementable effect In blueprint it will do spawn effect

	}

	// if we don't have energy 
	else
	{
		// say warning. 
	}

}



void ABaseTorch::TorchCrank()
{
	IncreaseEnergy();
	
}
void ABaseTorch::TorchOnOff(bool bSpotLightVisiblity)
{
	TorchSpotlight->SetVisibility(bSpotLightVisiblity);
	
	if (bSpotLightVisiblity == true)
	{
		GetWorldTimerManager().SetTimer(StartReducingEnergyTimerHandle, this, &ABaseTorch::DrainTorchEnergy, ReductionTimerRate, true);
	}
}

