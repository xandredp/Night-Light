// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/No_Bark_Vs.h"
#include "LightsSwitch.h"
#include "Assets/Actor_Light.h"
#include "Player/PlayController.h"




ALightsSwitch::ALightsSwitch()
{
	ItemID = FName("Please EnterID");
}

void ALightsSwitch::Interact(APlayerController* playerController)
{
	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{
		if (LightActorComp)
		{
			ToggleSwitchOnAndOff();
		}
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "PointLight Not assigned");
		}

	}
}

void ALightsSwitch::BeginPlay()
{
	Super::BeginPlay();

	//if (LightActorComp)
	//{
	//	LightActorComp->bIsLightOn = true;
	//	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Light Turned On");
	//	LightActorComp->ToggleSpotLightComp(true);
	//	LightingOnStart();
	//}


}

void ALightsSwitch::DestroyItemOnGround()
{
	Destroy();
}

void ALightsSwitch::ToggleSwitchOnAndOff()
{
}

