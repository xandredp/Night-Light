// Fill out your copyright notice in the Description page of Project Settings.
#include "TorchAttachmentPickUp.h"
#include "Core/No_Bark_Vs.h"
#include "Player/PlayController.h"






ATorchAttachmentPickUp::ATorchAttachmentPickUp(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemID = FName("Please EnterID");
}

void ATorchAttachmentPickUp::Interact(APlayerController* playerController)
{
	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{
		if (SoundPickUp != nullptr)
		{
			PlaySound(SoundPickUp);
		}
		//PlayPickupAnimation
		ANBCharacter* MyPawn = Cast<ANBCharacter>(aPlayController->GetPawn());
		if (MyPawn)
		{
			MyPawn->PlayPickUpAnimation();
			MyPawn->SpawnTorch();
		}
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
