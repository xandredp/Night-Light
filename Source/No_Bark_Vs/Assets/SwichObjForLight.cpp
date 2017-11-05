// Fill out your copyright notice in the Description page of Project Settings.
#include "Assets/SwichObjForLight.h"
#include "Core/No_Bark_Vs.h"
#include "Assets/CrackingObjForLight.h"
#include "Assets/Actor_Light.h"
#include "Player/PlayController.h"



ASwichObjForLight::ASwichObjForLight(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemID = FName("Please EnterID");
	EnergyTimerRate = 0.2f;	

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
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "PointLight Not assigned");
		}
		
	}
}

void ASwichObjForLight::BeginPlay()
{
	Super::BeginPlay();

	if (LightActorComp)
	{
		LightActorComp->bIsLightOn = true;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Light Turned On");
		LightActorComp->ToggleSpotLightComp(true);
		LightingOnStart();
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
			LightActorComp->bIsLightOn = false;
		//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Light off :  No EnergyLeft");

			LightActorComp->ToggleSpotLightComp(false);
			LightingOnStop();
		}

		//There is energy
		else
		{
			// Turn off light
			if (LightActorComp->bIsLightOn)
			{
				LightActorComp->bIsLightOn = false;
			//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Light Turned off");

				LightActorComp->ToggleSpotLightComp(false);
				LightingOnStop();

			}
			// Turn On light
			else
			{
				LightActorComp->bIsLightOn = true;
			//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Light Turned On");
				LightActorComp->ToggleSpotLightComp(true);
				LightingOnStart();
			}

		}
	}
	else
	{
	//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "Crackingobj Not assigned");
	}
}

void ASwichObjForLight::DecreaseEnergy()
{
	if (aCrackingObjForLight)
	{
		//Check if the enegrgy is what
		if (aCrackingObjForLight->CheckbIsEnergyZero())
		{
			aCrackingObjForLight->Energy = 0;
			LightActorComp->bIsLightOn = false;
			LightActorComp->ToggleSpotLightComp(LightActorComp->bIsLightOn);
			LightingOnStop();
			//do nothing
		}
		else
		{	//if energy is not zero then decrease. 
			float  DeductEnergyVal = aCrackingObjForLight->DeductEnergyBy;
			aCrackingObjForLight->DecreaseEnergyByFloat(DeductEnergyVal);
		}
	}
	
}

void ASwichObjForLight::IncreaseEnergy()
{
	float  AddEnergyVal = aCrackingObjForLight->AddEnergyBy; 
	aCrackingObjForLight->IncreaseEnergyByFloat(AddEnergyVal);
}

void ASwichObjForLight::LightingOnStart()
{
	if (LightActorComp->bIsLightOn == true)
	{
		GetWorldTimerManager().SetTimer(StartLightingTimerHandle, this, &ASwichObjForLight::DecreaseEnergy, EnergyTimerRate, true);
	}
}

void ASwichObjForLight::LightingOnStop()
{
	GetWorldTimerManager().ClearTimer(StartLightingTimerHandle);
	LightActorComp->ToggleSpotLightComp(false);
}

