// Fill out your copyright notice in the Description page of Project Settings.
#include "Core/Pickup.h"
#include "Core/No_Bark_Vs.h"
#include "Player/PlayController.h"


APickup::APickup()
{


	ItemID = FName("Please EnterID");

}

void APickup::Interact(APlayerController* playerController)
{

//	Super::Interact(playerController);
	//bool bNoSpaceInInventory = false;
	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{

		DestroyItemOnGround();
	
	}

	
}

void APickup::DestroyItemOnGround()
{
	Destroy();
}

