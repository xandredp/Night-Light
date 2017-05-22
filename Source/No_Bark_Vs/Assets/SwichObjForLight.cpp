// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "CrackingObjForLight.h"
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
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "SwitchFlicked");

		
		if (aCrackingObjForLight)
		{
			if (aCrackingObjForLight->CheckbIsEnergyZero())
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Light off No EnergyLeft");
			}
			else
			{
				if (bIsLightOn)
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Light Turned off");
				
				}
				else
				{
					GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Light Turned On");
				}
			
			}
		}
	}
}

void ASwichObjForLight::DestroyItemOnGround()
{
	Destroy();
}



