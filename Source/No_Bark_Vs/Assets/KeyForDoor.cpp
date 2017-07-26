// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/No_Bark_Vs.h"
#include "Player/PlayController.h"
#include "KeyForDoor.h"




AKeyForDoor::AKeyForDoor()
{
	ItemID = FName("Please EnterID");
}

void AKeyForDoor::Interact(APlayerController* playerController)
{
	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{
		////	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Interaction Completed");
		//aPlayController->AddItemtoInventoryByID(ItemID, 1);
		DestroyItemOnGround();
	}
}

void AKeyForDoor::BeginPlay()
{
	Super::BeginPlay();
	ItemID = KeyConfig.KeyID;
}

void AKeyForDoor::DestroyItemOnGround()
{
	Destroy();
}
