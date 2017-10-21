// Fill out your copyright notice in the Description page of Project Settings.

#include "InstructionPickUp.h"
#include "Player/PlayController.h"
#include "Player/NBCharacter.h"


AInstructionPickUp::AInstructionPickUp()
{
	ItemID = FName("Please EnterID");
}

void AInstructionPickUp::Interact(APlayerController * playerController)
{
	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{
		//PlayPickupAnimation
		ANBCharacter* MyPawn = Cast<ANBCharacter>(aPlayController->GetPawn());
		if (MyPawn)
		{
			MyPawn->PlayPickUpAnimation();

			aPlayController->AddCurrentInstruction(ItemID);
			if (aPlayController->isBookWidgetOpen)
			{
				aPlayController->CloseBookWidget();
			}
			else
			{
				aPlayController->OpenBookWidget();
			}
			
		}

		//DestroyItemOnGround();
	}
}

void AInstructionPickUp::BeginPlay()
{
	Super::BeginPlay();
}

void AInstructionPickUp::DestroyItemOnGround()
{
	Destroy();
}
