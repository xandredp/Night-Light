// Fill out your copyright notice in the Description page of Project Settings.
#include "Door.h"
#include "Core/No_Bark_Vs.h"
#include "Player/PlayController.h"


ADoor::ADoor()
{
	ItemID = FName("Please EnterID");
	IsDoorOpen = false;
	IsDoorLock = true;
	IsDoubleDoor = false;
	LockDoorForSpecificMechanic = false;
}

void ADoor::Interact(APlayerController* playerController)
{
	if(LockDoorForSpecificMechanic ==  true)
	{
		Action = "Power needed";
	}
	else
	{
		APlayController* aPlayController = Cast<APlayController>(playerController);
		if (aPlayController)
		{
			if (IsDoubleDoor)
			{
				if (AnotherDoor)
				{
					AnotherDoor->ToggleDoor();
				}
			}
			ToggleDoor();
		}
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
