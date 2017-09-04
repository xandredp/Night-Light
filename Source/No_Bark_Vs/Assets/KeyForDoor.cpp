// Fill out your copyright notice in the Description page of Project Settings.
#include "KeyForDoor.h"
#include "Core/No_Bark_Vs.h"
#include "Player/PlayController.h"
#include "Player/NBCharacter.h"




AKeyForDoor::AKeyForDoor()
{
	ItemID = FName("Please EnterID");
}

void AKeyForDoor::Interact(APlayerController* playerController)
{
	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{
		//PlayPickupAnimation
		ANBCharacter* MyPawn = Cast<ANBCharacter>(aPlayController->GetPawn());
		if (MyPawn)
		{
			MyPawn->PlayPickUpAnimation();
		}
		aPlayController->AddKeytoPossesion(KeyConfig);
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
