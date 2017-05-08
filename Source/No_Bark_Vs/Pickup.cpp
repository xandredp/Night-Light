// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "PlayController.h"
#include "Pickup.h"

APickup::APickup()
{


	ItemID = FName("Please EnterID");

}

void APickup::Interact(APlayerController* playerController)
{

//	Super::Interact(playerController);
	bool bNoSpaceInInventory = false;
	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Interactiion Successed");
		aPlayController->AddItemtoInventoryByID(ItemID, 1);		
		bNoSpaceInInventory = aPlayController->GetIsInventoryFull();
		if (bNoSpaceInInventory == false)
		{
			DestroyItemOnGround();
		}
	
	}

	
}

void APickup::DestroyItemOnGround()
{
	Destroy();
}

