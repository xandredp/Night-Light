// Fill out your copyright notice in the Description page of Project Settings.
#include "LightsSwitch.h"
#include "Core/No_Bark_Vs.h"

#include "Assets/Actor_Light.h"
#include "Player/PlayController.h"




ALightsSwitch::ALightsSwitch()
{
	ItemID = FName("Please EnterID");
	LockSwitchForSpecificMechanic = false;
}

void ALightsSwitch::Interact(APlayerController* playerController)
{
	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{
		if (LightActors.Num() != 0)
		{
			//ToggleSwitchOnAndOff();
			InteractionFeedBack();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Light Not assigned");
		}
	}
}

void ALightsSwitch::BeginPlay()
{
	Super::BeginPlay();
	if (LightActors.Num() != 0)
	{
		IsSwitchOn = false;
		for (int32 i = 0; i < LightActors.Num(); i++)
		{
			LightActors[i]->bIsLightOn = false;

			//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Light Turned off");

			LightActors[i]->ToggleSpotLightComp(false);
		}
	}
}

void ALightsSwitch::DestroyItemOnGround()
{
	Destroy();
}

void ALightsSwitch::ToggleSwitchOnAndOff()
{

	for (int32 i = 0; i < LightActors.Num(); i++)
	{
		//Turn Off Light
		if (LightActors[i]->bIsLightOn)
		{
			LightActors[i]->bIsLightOn = false;
			//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Light Turned off");

			LightActors[i]->ToggleSpotLightComp(false);
		}
		// Turn On light
		else
		{
			LightActors[i]->bIsLightOn = true;
			//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Light Turned On");
			LightActors[i]->ToggleSpotLightComp(true);
		}
	}
}

