// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/No_Bark_Vs.h"
#include "Player/PlayController.h"
#include "Door.h"
#include "TorchAttachmentPickUp.h"




ATorchAttachmentPickUp::ATorchAttachmentPickUp()
{
	ItemID = FName("Please EnterID");
}

void ATorchAttachmentPickUp::Interact(APlayerController* playerController)
{
	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{
		aPlayController->SetIsTorchOn(true);
		DoorToInteract->AutoCloseLock();
		BeginWithTorchOn();
		DestroyItemOnGround();
	
	}
}

void ATorchAttachmentPickUp::BeginPlay()
{
	Super::BeginPlay();
}

void ATorchAttachmentPickUp::DestroyItemOnGround()
{
	Destroy();
}
