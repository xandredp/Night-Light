// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "PlayController.h"
#include "Kismet/HeadMountedDisplayFunctionLibrary.h"
#include "Engine.h"
#include "CrackingObjForLight.h"



ACrackingObjForLight::ACrackingObjForLight()
{
	ItemID = FName("Please EnterID");
	Energy = 10.0f;
	MaxEnergy = 100.0f;
	AddEnergyBy = 10.0f;
	DeductEnergyBy = 1.0f;
	bIsEnergyZero = false;
	bIsEnergyMaxed = false;
	EnergyTimerRate = 0.2f;
}

void ACrackingObjForLight::Interact(APlayerController* playerController)
{
	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Interaction Successed");

		IncreaseEnergy();
	}
}

void ACrackingObjForLight::DestroyItemOnGround()
{
	Destroy();
}

void ACrackingObjForLight::DecreaseEnergy()
{
	DecreaseEnergyByFloat(DeductEnergyBy);
}

void ACrackingObjForLight::IncreaseEnergy()
{
	IncreaseEnergyByFloat(AddEnergyBy);
}

void ACrackingObjForLight::DecreaseEnergyByFloat(float deductBy)
{
	if (!CheckbIsEnergyZero())
	{
		Energy -= deductBy;
	}
}

void ACrackingObjForLight::IncreaseEnergyByFloat(float addBy)
{
	if (!CheckbIsEnergyMaxed())
	{
		Energy += addBy;
	}
}

bool ACrackingObjForLight::CheckbIsEnergyZero()
{
	if (Energy > 0.0f)
	{
		bIsEnergyZero = false;
	}
	else
	{
		Energy = 0.0f;
		bIsEnergyZero = true;
	}
	return bIsEnergyZero;
}

bool ACrackingObjForLight::CheckbIsEnergyMaxed()
{
	if (Energy >= MaxEnergy)
	{
		bIsEnergyMaxed = true;
	}
	else
	{
		bIsEnergyMaxed = false;
	}
	return bIsEnergyMaxed;
}

void ACrackingObjForLight::LightingOnStart()
{
	GetWorldTimerManager().SetTimer(StartLightingTimerHandle, this, &ACrackingObjForLight::DecreaseEnergy, EnergyTimerRate, true);
}

void ACrackingObjForLight::LightingOnStop()
{
	GetWorldTimerManager().ClearTimer(StartLightingTimerHandle);
}



