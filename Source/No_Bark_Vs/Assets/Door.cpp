// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/No_Bark_Vs.h"
#include "Player/PlayController.h"
#include "Door.h"





ADoor::ADoor()
{
	ItemID = FName("Please EnterID");
}

void ADoor::Interact(APlayerController* playerController)
{
	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{
		ToggleDoor();
	}
}

void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADoor::DestroyItemOnGround()
{
	Destroy();
}
