// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "CrackingObjForLight.h"
#include "PlayController.h"
#include "SwichObjForLight.h"






ASwichObjForLight::ASwichObjForLight()
{
	ItemID = FName("Please EnterID");
}

void ASwichObjForLight::Interact(APlayerController* playerController)
{
	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, "SwitchFlicked");
		//ACrackingObjForLight* aCrackingObjForLight = Cast<ACrackingObjForLight>
	}
}

void ASwichObjForLight::DestroyItemOnGround()
{
	Destroy();
}



