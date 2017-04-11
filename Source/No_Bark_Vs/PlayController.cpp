// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "BaseInteractable.h"
#include "NBCharacter.h"
#include "PlayGameMode.h"
#include "PlayController.h"




void APlayController::Interact()
{
	if (CurrentInteractable)
	{
		CurrentInteractable->Interact(this);

	}
}

void APlayController::AddItemtoInventoryByID(FName ID)
{
	APlayGameMode* PlayGameMode = Cast<APlayGameMode>(GetWorld()->GetAuthGameMode());
	UDataTable* ItemTable = PlayGameMode->GetItemDB();
	
	FInventoryItem* ItemToADD = ItemTable->FindRow<FInventoryItem>(ID, "");
	
	if (ItemToADD)
	{
		Inventory.Add(*ItemToADD);
	}


}

void APlayController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Use", IE_Pressed, this, &APlayController::Interact);


}
