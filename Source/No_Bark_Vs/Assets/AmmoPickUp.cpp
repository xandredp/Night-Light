// Fill out your copyright notice in the Description page of Project Settings.
#include "AmmoPickUp.h"
#include "Core/No_Bark_Vs.h"
#include "Player/PlayController.h"
#include "Player/NBCharacter.h"






AAmmoPickUp::AAmmoPickUp()
{
	ItemID = FName("Please EnterID");
	ClipsInBox = 5;
}

void AAmmoPickUp::Interact(APlayerController* playerController)
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
		}
		PickupClips();
	}
}

void AAmmoPickUp::BeginPlay()
{
	Super::BeginPlay();

}

void AAmmoPickUp::DestroyItemOnGround()
{
	Destroy();
}
