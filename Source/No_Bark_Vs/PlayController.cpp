// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "BaseInteractable.h"
#include "NBCharacter.h"
#include "TypeClass.h"
#include "PlayGameMode.h"
#include "Blueprint/UserWidget.h"
#include "PlayController.h"




APlayController::APlayController()
{
	MaxInventorySize = 5;
	LastAddedInventoryIndex = 0;

}

void APlayController::Interact()
{//ABaseInteractable


	if (CurrentInteractable)
	{
		CurrentInteractable->Interact(this);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::FromInt(FCurrentInventory.Num()));

	}
}

void APlayController::CraftItem(FInventoryItem ItemA, FInventoryItem ItemB, APlayController* PlayController)
{
	for (auto Craft : ItemB.CraftCombinations)
	{
		if (Craft.ComponentID == ItemA.ItemID)
		{
			if (Craft.bDestroyItemA)
			{
				//Inventory.RemoveSingle(ItemA)
			}
			if (Craft.bDestroyItemB)
			{
				//Inventory.RemoveSingle(ItemB);
			}

			AddItemtoInventoryByID(Craft.ProductID);

			ReloadInventory();
		}
	}
}

void APlayController::SetInputModetoGameandUI(bool bHideCursor)
{
	FInputModeGameAndUI InputMode;
	FInputModeGameOnly GameOnlyInputMode;
	if (bHideCursor == true)
	{
		InputMode.SetHideCursorDuringCapture(bHideCursor);
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);
	}
	else
	{
		SetInputMode(GameOnlyInputMode);
	}

}

void APlayController::ChangeMaxInventorySize(int iNoInventory)
{
	MaxInventorySize = iNoInventory;
}

void APlayController::AddItemtoInventoryByID(FName ID)
{
	// getting the game mode and get Item database. 
	APlayGameMode* PlayGameMode = Cast<APlayGameMode>(GetWorld()->GetAuthGameMode());
	UDataTable* ItemTable = PlayGameMode->GetItemDB();	
	// find inventory item. 
	FInventoryItem* ItemToADD = ItemTable->FindRow<FInventoryItem>(ID, "");
	FCurrentInventoryItemInfo CurrentItemToAddInventory;
	CurrentItemToAddInventory.CurrentStackNumber = 1;
	CurrentItemToAddInventory.ItemInfo = *ItemToADD;
	CurrentItemToAddInventory.ItemIndex = LastAddedInventoryIndex;
	bool bItemAdded = false;
	// if inventory item is valid add to inventory. 
	if (ItemToADD)
	{
		do // do until the Item is added
		{
			if (FCurrentInventory.Num()==0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Item has been added as index  0 is  false ");
				LastAddedInventoryIndex = FCurrentInventory.AddUnique(CurrentItemToAddInventory);
				bItemAdded = true;
		
			}
			else
			{
				// Search all items to check if item already exist, if it does increment current stack number.
				for (int32 i = 0; i < FCurrentInventory.Num(); i++)
				{
					if (FCurrentInventory[i].ItemInfo.ItemID == ItemToADD->ItemID)
					{
						if (FCurrentInventory[i].CurrentStackNumber < FCurrentInventory[i].ItemInfo.MaxStackNumber)
						{
							FCurrentInventory[i].CurrentStackNumber = FCurrentInventory[i].CurrentStackNumber + 1;
							
							FString string;
							string = "Item has been added CurrentstackNumber :" + FString::FromInt(FCurrentInventory[i].CurrentStackNumber);
							GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, string );
							
							bItemAdded = true;
						}
					}
				}
				//if same item doesn't exist Add unique item 
				if (bItemAdded == false)
				{				
					if (LastAddedInventoryIndex >= MaxInventorySize)
					{
						///Todo : Warning Inventory is full and remove the item. 
					}
					else
					{
						FString string;
						string = "Item was full CurrentstackNumber :";

						LastAddedInventoryIndex = FCurrentInventory.Add(CurrentItemToAddInventory);
						FCurrentInventory[LastAddedInventoryIndex].ItemIndex = LastAddedInventoryIndex;
					}

					bItemAdded = true;
				}

			}

		} while (bItemAdded != true);

		
	}

	ReloadInventory();
}

void APlayController::UseItem(FCurrentInventoryItemInfo iItemInfo)
{

}

void APlayController::UnUseItem(FCurrentInventoryItemInfo iItemInfo)
{
}

void APlayController::EnableActionBar(FCurrentInventoryItemInfo iItemInfo)
{
}

void APlayController::OpenInventory()
{
	if (isMyInventoryOpen == true)
	{
		CloseInventory();
		isMyInventoryOpen = false;
		SetInputModetoGameandUI(false);
		
	}
	else
	{
		if (wInventory) // Check if the Asset is assigned in the blueprint.
		{

			// Create the widget and store it.
			MyInventoryWidget = CreateWidget<UUserWidget>(this, wInventory);
			MyEquipmentWidget = CreateWidget<UUserWidget>(this, wEquipment);
			// now you can use the widget directly since you have a referance for it.
			// Extra check to  make sure the pointer holds the widget.
			if (MyInventoryWidget)
			{
				SetInputModetoGameandUI(true);
				MyInventoryWidget->AddToViewport(1);
				MyEquipmentWidget->AddToViewport(1);
			}
		}

		isMyInventoryOpen = true;
	}
	
	
}

void APlayController::CloseInventory()
{
	MyInventoryWidget->RemoveFromParent();
	MyEquipmentWidget->RemoveFromParent();
}

void APlayController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Use", IE_Pressed, this, &APlayController::Interact);
	InputComponent->BindAction("Inventory", IE_Pressed, this, &APlayController::OpenInventory);


}
