// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "BaseInteractable.h"
#include "NBCharacter.h"
#include "PlayGameMode.h"
#include "Blueprint/UserWidget.h"
#include "PlayController.h"




APlayController::APlayController()
{
}

void APlayController::Interact()
{
	if (CurrentInteractable)
	{
		CurrentInteractable->Interact(this);

	}
}

void APlayController::AddItemtoInventoryByID(FName ID)
{
	// getting the game mode and get Item database. 
	APlayGameMode* PlayGameMode = Cast<APlayGameMode>(GetWorld()->GetAuthGameMode());
	UDataTable* ItemTable = PlayGameMode->GetItemDB();
	
	// find inventory item. 
	FInventoryItem* ItemToADD = ItemTable->FindRow<FInventoryItem>(ID, "");
	
	// if inventory item is valid add to inventory. 
	if (ItemToADD)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Item has been added");
		//Inventory.Add(*ItemToADD);
		Inventory.AddUnique(*ItemToADD);
	}


}

void APlayController::OpenInventory()
{
	if (isMyInventoryOpen == true)
	{
		MyInventory->RemoveFromParent();
		isMyInventoryOpen = false;
	}
	else
	{
		if (wInventory) // Check if the Asset is assigned in the blueprint.
		{

			// Create the widget and store it.
			MyInventory = CreateWidget<UUserWidget>(this, wInventory);
			// now you can use the widget directly since you have a referance for it.
			// Extra check to  make sure the pointer holds the widget.
			if (MyInventory)
			{
				FInputModeGameAndUI InputMode;
				InputMode.SetHideCursorDuringCapture(true);
				InputMode.SetLockMouseToViewport(false);

				//let add it to the view port
				MyInventory->AddToViewport(1);
				/*SetInputMode(InputMode);*/

			}

			//Show the Cursor.
			bShowMouseCursor = true;
		}

		isMyInventoryOpen = true;
	}
	
	
}

void APlayController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Use", IE_Pressed, this, &APlayController::Interact);
	InputComponent->BindAction("Inventory", IE_Pressed, this, &APlayController::OpenInventory);


}
