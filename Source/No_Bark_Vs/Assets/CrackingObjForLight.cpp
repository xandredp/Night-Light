// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "PlayController.h"
#include "CrackingObjForLight.h"



ACrackingObjForLight::ACrackingObjForLight()
{


	ItemID = FName("Please EnterID");
	Energy = 10;
	MaxEnergy = 100;
	AddEnergyBy = 10;
	DeductEnergyBy = 1;
	IsLightOn = false;
	IsEnergyZero = false;
}

void ACrackingObjForLight::Interact(APlayerController* playerController)
{
	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Interaction Successed");

		if (Energy <= MaxEnergy)
		{
			Energy = Energy + 10;
		}
	}
}

void ACrackingObjForLight::DestroyItemOnGround()
{
	Destroy();
}



