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
	MaxEquipmentSize = 6;
	LastAddedInventoryIndex = 0;
	isMyMapOpen = false;
	isMyInventoryOpen = false;
	bShowMouseCursor = false;
	FCurrentEquippedMeleeWeapon.CurrentStackNumber = 0;
	FCurrentEquippedMeleeWeapon.ItemInfo.eItemType = EItemType::MeleeWeapon;
}

void APlayController::Interact()
{//ABaseInteractable
	if (CurrentInteractable)
	{

		CurrentInteractable->Interact(this);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::FromInt(FCurrentInventory.Num()));

	}
}

void APlayController::OpenInventory()
{
	if (isMyInventoryOpen == true)
	{
		CloseInventory();
		
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
				MyInventoryWidget->bIsFocusable = true;
				bShowMouseCursor = true;
				MyInventoryWidget->AddToViewport(1);
				MyEquipmentWidget->AddToViewport(1);
			}
		}

		isMyInventoryOpen = true;
	}


}

void APlayController::OpenMap()
{

	if (isMyMapOpen == true)
	{
		MyMapWidget->RemoveFromParent();
		isMyMapOpen = false;
	}
	else
	{
		if (wMinimap) // Check if the Asset is assigned in the blueprint.
		{
			MyMapWidget = CreateWidget<UUserWidget>(this, wMinimap);
			// now you can use the widget directly since you have a referance for it.
			// Extra check to  make sure the pointer holds the widget.
			if (MyMapWidget)
			{
				MyMapWidget->AddToViewport(1);
			}
		}

		isMyMapOpen = true;
	}
}

void APlayController::UseItem(FCurrentInventoryItemInfo iItemInfo)
{
	EItemType eItemType = iItemInfo.ItemInfo.eItemType;
	if (iItemInfo.CurrentStackNumber != 0)
	{
		if (iItemInfo.ItemInfo.eItemType != EItemType::None)
		{
			switch (eItemType)
			{
			case EItemType::Weapon:
				break;

			case EItemType::MeleeWeapon:
				break;

			case EItemType::Armor:
				break;

			case EItemType::Grenades:
				break;

			case EItemType::Food:
				break;

			case EItemType::Potion:
				break;

			default:
				break;
			}
		}
	}

}

void APlayController::UnUseItem(FCurrentInventoryItemInfo iItemInfo)
{
}

void APlayController::ReArrangeItems()
{
	for (int32 i = 0; i < FCurrentInventory.Num(); i++)
	{
		FCurrentInventory[i].ItemIndex = i;
	}

	for (int32 i = 0; i < FCurrentEquippedWeapons.Num(); i++)
	{
		FCurrentEquippedWeapons[i].ItemIndex = i;
	}

	for (int32 i = 0; i < FCurrentEquipment.Num(); i++)
	{
		FCurrentEquipment[i].ItemIndex = i;
	}
}

void APlayController::DropItem(FCurrentInventoryItemInfo iItemInfo)
{
}

void APlayController::CloseInventory()
{
	MyInventoryWidget->RemoveFromParent();
	MyEquipmentWidget->RemoveFromParent();

	isMyInventoryOpen = false;
	SetInputModetoGameandUI(false);
	bShowMouseCursor = false;
}

void APlayController::AddItemtoInventoryByID(FName ID, int ItemCurrentStackNumber)
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
	bIsInventoryFull = bItemAdded;
	// if inventory item is valid add to inventory. 
	if (ItemToADD)
	{
		do // do until the Item is added
		{
			if (FCurrentInventory.Num() == 0)
			{
				LastAddedInventoryIndex = FCurrentInventory.AddUnique(CurrentItemToAddInventory);
				FCurrentInventory[LastAddedInventoryIndex].CurrentStackNumber = ItemCurrentStackNumber;
				FCurrentInventory[LastAddedInventoryIndex].ItemIndex = LastAddedInventoryIndex;


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
							FCurrentInventory[i].CurrentStackNumber = FCurrentInventory[i].CurrentStackNumber + ItemCurrentStackNumber;

							FString string;
							string = "Item has been added CurrentstackNumber :" + FString::FromInt(FCurrentInventory[i].CurrentStackNumber);
							GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, string);

							bItemAdded = true;
						}
					}
				}
				//if same item doesn't exist Add unique item 
				if (bItemAdded == false)
				{
					if (FCurrentInventory.Num() >= MaxInventorySize)
					{
						///Todo : Warning Inventory is full and remove the item. 
						GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "InventoryIsFull");
						bIsInventoryFull = true;

					}
					else
					{
						FString string;
						string = "Item was full CurrentstackNumber :";

						LastAddedInventoryIndex = FCurrentInventory.Add(CurrentItemToAddInventory);
						FCurrentInventory[LastAddedInventoryIndex].ItemIndex = LastAddedInventoryIndex;
						FCurrentInventory[LastAddedInventoryIndex].CurrentStackNumber = ItemCurrentStackNumber;
					}

					bItemAdded = true;
				}

			}

		} while (bItemAdded != true);


	}

	ReloadInventory();
}

void APlayController::MoveItemWithinInventory(int ItemAIndex, int ItemBIndex)
{
	FCurrentInventoryItemInfo ItemFrom;
	FCurrentInventoryItemInfo ItemTo;

	if (FCurrentInventory[ItemAIndex].CurrentStackNumber == 0)
	{

	}
	else
	{
		ItemFrom = FCurrentInventory[ItemAIndex];

		if (FCurrentInventory[ItemBIndex].CurrentStackNumber == 0)
		{
			
		}
		else if (FCurrentInventory.Num() == ItemAIndex)
		{

		}
		else
		{
			ItemTo = FCurrentInventory[ItemBIndex];
			
			FCurrentInventory[ItemBIndex].CurrentStackNumber = ItemFrom.CurrentStackNumber;
			FCurrentInventory[ItemBIndex].ItemInfo = ItemFrom.ItemInfo;
			FCurrentInventory[ItemBIndex].ItemIndex = ItemBIndex;

			FCurrentInventory[ItemAIndex].CurrentStackNumber = ItemTo.CurrentStackNumber;
			FCurrentInventory[ItemAIndex].ItemInfo = ItemTo.ItemInfo;
			FCurrentInventory[ItemAIndex].ItemIndex = ItemAIndex;

		}
	}

	ReloadInventory();
}

void APlayController::RemoveItemFromInventory(FCurrentInventoryItemInfo ItemToRemove)
{//Item has been used Remove 1Stack Number From Inventory.
	int relatedindex = ItemToRemove.ItemIndex;
	if (ItemToRemove.CurrentStackNumber == 1)
	{
		FCurrentInventory.RemoveAt(relatedindex);
		
		ReloadInventory();

		ReArrangeItems();


	}
	else if (ItemToRemove.CurrentStackNumber > 1)
	{
		FCurrentInventory[relatedindex].CurrentStackNumber = FCurrentInventory[relatedindex].CurrentStackNumber -1;
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

			AddItemtoInventoryByID(Craft.ProductID, 1);

			ReloadInventory();
		}
	}
}

//private?
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

bool APlayController::GetIsInventoryFull()
{
	return bIsInventoryFull;
}

void APlayController::AttachEquipmenttoCharacter(FCurrentInventoryItemInfo ItemToAttech)
{
}

void APlayController::DetachEquipmentfromCharacter(FCurrentInventoryItemInfo ItemToDetach)
{
}

void APlayController::RemoveItemFromEquipment(FCurrentInventoryItemInfo ItemtoRemove)
{
	int iIndex = ItemtoRemove.ItemIndex;
	FCurrentEquipment[iIndex].CurrentStackNumber = 0;
	FCurrentEquipment[iIndex].ItemInfo.eItemType = EItemType::None;
}
void APlayController::MoveItemToInventory(FCurrentInventoryItemInfo iItemFromEqupment, int ItemBIndex)
{

	FCurrentInventoryItemInfo ItemFrom;
	FCurrentInventoryItemInfo ItemTo;
	int CurrentIndex;

	if (iItemFromEqupment.CurrentStackNumber == 0)
	{
		// do nothing just remoive item B.
	}
	else
	{
		ItemFrom = iItemFromEqupment;
		CurrentIndex = FCurrentInventory.Add(iItemFromEqupment);
		FCurrentInventory[CurrentIndex].ItemIndex = CurrentIndex;
	}
	ReloadInventory();
}

void APlayController::AddItemtoEquipmentByItem(FCurrentInventoryItemInfo iItemtoAdd, int toIndex)
{
	int LastAddedEquipmentyIndex = 0;
	bool bItemAdded = false;
	bool bItemRemovedinInventory = false;
	EItemType iItemType = iItemtoAdd.ItemInfo.eItemType;
	iItemRemovedfromEquipment.CurrentStackNumber = 0;
	// if inventory item is valid add to inventory. 
	do // do until the Item is added
	{
		if (iItemType==EItemType::None)
		{
		}
		else if (iItemType == EItemType::Weapon)
		{

			if (FCurrentEquippedWeapons.Num() <= 1)
			{
				LastAddedEquipmentyIndex = FCurrentEquippedWeapons.Add(iItemtoAdd);
				FCurrentEquippedWeapons[LastAddedEquipmentyIndex].ItemIndex = LastAddedEquipmentyIndex;

				bItemAdded = true;
			}
			else if (toIndex <= 1)
			{
				LastAddedEquipmentyIndex = toIndex;
				iItemRemovedfromEquipment = FCurrentEquippedWeapons[LastAddedEquipmentyIndex];
				FCurrentEquippedWeapons[LastAddedEquipmentyIndex] = iItemtoAdd;
				FCurrentEquippedWeapons[LastAddedEquipmentyIndex].ItemIndex = LastAddedEquipmentyIndex;

				bItemAdded = true;
			}
			else
			{
				LastAddedEquipmentyIndex = 1;
				iItemRemovedfromEquipment = FCurrentEquippedWeapons[LastAddedEquipmentyIndex];
				FCurrentEquippedWeapons[LastAddedEquipmentyIndex] = iItemtoAdd;
				FCurrentEquippedWeapons[LastAddedEquipmentyIndex].ItemIndex = LastAddedEquipmentyIndex;

				bItemAdded = true;
			}
		}

		else if (iItemType == EItemType::MeleeWeapon)
		{
			iItemRemovedfromEquipment = FCurrentEquippedMeleeWeapon;
			FCurrentEquippedMeleeWeapon = iItemtoAdd;

			bItemAdded = true;
		}
		//// if  EItemType::Not Weapon...
		else
		{

			if (FCurrentEquipment.Num() <= 2)
			{
				LastAddedEquipmentyIndex = FCurrentEquipment.Add(iItemtoAdd);
				FCurrentEquipment[LastAddedEquipmentyIndex].ItemIndex = LastAddedEquipmentyIndex;

				bItemAdded = true;
			}
			else if (toIndex <= 2)
			{
				LastAddedEquipmentyIndex = toIndex;
				iItemRemovedfromEquipment = FCurrentEquipment[LastAddedEquipmentyIndex];
				FCurrentEquipment[LastAddedEquipmentyIndex] = iItemtoAdd;
				FCurrentEquipment[LastAddedEquipmentyIndex].ItemIndex = LastAddedEquipmentyIndex;

				bItemAdded = true;
			}
			else
			{
				LastAddedEquipmentyIndex = 2;
				iItemRemovedfromEquipment = FCurrentEquipment[LastAddedEquipmentyIndex];
				FCurrentEquipment[LastAddedEquipmentyIndex] = iItemtoAdd;
				FCurrentEquipment[LastAddedEquipmentyIndex].ItemIndex = LastAddedEquipmentyIndex;

				bItemAdded = true;
			}
		}

		bItemAdded = true;
	} while (bItemAdded != true);

	ReArrangeItems();

	// Remove Iteam. 
	for (int32 j = 0; j < FCurrentInventory.Num(); j++)
	{
		if (FCurrentInventory[j].ItemInfo.ItemID == iItemtoAdd.ItemInfo.ItemID)
		{
			if (FCurrentInventory[j].CurrentStackNumber == iItemtoAdd.CurrentStackNumber)
			{
				if (bItemRemovedinInventory == false)
				{
					FCurrentInventory.RemoveAt(j);
					bItemRemovedinInventory = true;
				}
			}
		}
	}

	if (iItemRemovedfromEquipment.CurrentStackNumber > 0)
	{
		AddItemtoInventoryByID(iItemRemovedfromEquipment.ItemInfo.ItemID, iItemRemovedfromEquipment.CurrentStackNumber);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "stacknumber higherthan 0");
	}
	ReloadInventory();
}

void APlayController::AttachEquipment(FCurrentInventoryItemInfo iItemInfo)
{
	EItemType eItemType = iItemInfo.ItemInfo.eItemType;
	if (iItemInfo.CurrentStackNumber != 0)
	{
		if (iItemInfo.ItemInfo.eItemType != EItemType::None)
		{
			switch (eItemType)
			{
			case EItemType::Weapon:
				break;

			case EItemType::MeleeWeapon:
				break;

			case EItemType::Armor:
				break;

			case EItemType::Grenades:
				break;

			case EItemType::Food:
				break;

			case EItemType::Potion:
				break;	

			default:
				break;
			}
		}
	}
	
}

void APlayController::UnAttachEquipment(FCurrentInventoryItemInfo iItemInfo)
{
}

void APlayController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Use", IE_Pressed, this, &APlayController::Interact);
	InputComponent->BindAction("Inventory", IE_Pressed, this, &APlayController::OpenInventory);
	InputComponent->BindAction("OpenMap", IE_Pressed, this, &APlayController::OpenMap);


}
