// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "CrackingObjForLight.h"
#include "Actor_Light.h"
#include "PlayController.h"
#include "SwichObjForLight.h"


ASwichObjForLight::ASwichObjForLight()
{
	ItemID = FName("Please EnterID");
	bIsLightOn = false;
}

void ASwichObjForLight::Interact(APlayerController* playerController)
{
	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{
		if (LightActorComp)
		{
			SwitchingOnAndOff();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "PointLight Not assigned");
		}
		
	}
}

void ASwichObjForLight::DestroyItemOnGround()
{
	Destroy();
}

void ASwichObjForLight::SwitchingOnAndOff()
{
	if (aCrackingObjForLight)
	{
		// If there is no energy left
		if (aCrackingObjForLight->CheckbIsEnergyZero())
		{
			bIsLightOn = false;
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Light off :  No EnergyLeft");

			LightActorComp->TogglePointLightComp(false);
		}

		//There is energy
		else
		{
			// Turn off light
			if (bIsLightOn)
			{
				bIsLightOn = false;
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Light Turned off");

				LightActorComp->TogglePointLightComp(false);

			}
			// Turn On light
			else
			{
				bIsLightOn = true;
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Light Turned On");
				LightActorComp->TogglePointLightComp(true);
			}

		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Crackingobj Not assigned");
	}
}



