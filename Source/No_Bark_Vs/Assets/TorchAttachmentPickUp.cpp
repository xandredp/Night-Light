// Fill out your copyright notice in the Description page of Project Settings.
#include "TorchAttachmentPickUp.h"
#include "Player/PlayController.h"






ATorchAttachmentPickUp::ATorchAttachmentPickUp()
{
	ItemID = FName("Please EnterID");
}

void ATorchAttachmentPickUp::Interact(APlayerController* playerController)
{
	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{
		//PlayPickupAnimation
		ANBCharacter* MyPawn = Cast<ANBCharacter>(aPlayController->GetPawn());
		if (MyPawn)
		{
			MyPawn->PlayPickUpAnimation();
			MyPawn->SpawnTorch();
		}

		aPlayController->SetIsTorchOn(true);
		//DoorToInteract->AutoCloseLock();
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
