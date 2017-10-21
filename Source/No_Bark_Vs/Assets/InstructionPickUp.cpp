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
		if (aPlayController->isBookWidgetOpen)
		{
			aPlayController->CloseBookWidget();
		}
		else
		{
			aPlayController->AddCurrentInstruction(ItemID);
			aPlayController->OpenBookWidget();
		}

		//PlayPickupAnimation
		ANBCharacter* MyPawn = Cast<ANBCharacter>(aPlayController->GetPawn());
		if (MyPawn)
		{
			MyPawn->PlayPickUpAnimation();
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
