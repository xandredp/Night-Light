// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/PlayController.h"
#include "Core/No_Bark_Vs.h"
#include "Core/BaseInteractable.h"
#include "Player/NBCharacter.h"
#include "Core/TypeClass.h"
#include "Core/PlayGameMode.h"
#include "Core/BaseTorch.h"
#include "Engine/DataTable.h"
#include "Blueprint/UserWidget.h"

APlayController::APlayController()
{
	bShowMouseCursor = false;
	MyCurrentCurrency = 0.0f;
	isBookWidgetOpen = false;
}

void APlayController::Interact()
{//ABaseInteractable

	if (isBookWidgetOpen)
	{
		CloseBookWidget();
	}
	if (CurrentInteractable)
	{
		CurrentInteractable->Interact(this);
	//	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::FromInt(FCurrentInventory.Num()));
	}
}
bool APlayController::IsInteract()
{//ABaseInteractable
	return (CurrentInteractable != nullptr);
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
void APlayController::OpenBookWidget()
{
	if (isBookWidgetOpen)
	{
		CloseBookWidget();
	}
	else
	{
		if (wBook)
		{
			wBookWidget = CreateWidget<UUserWidget>(this, wBook);
			if (wBookWidget)
			{
				SetInputModetoGameandUI(true);
				wBookWidget->bIsFocusable = true;
			//	bShowMouseCursor = true;
				wBookWidget->AddToViewport(1);
			}
		}

		isBookWidgetOpen = true;
	}
}
void APlayController::CloseBookWidget()
{
	wBookWidget->RemoveFromParent();

	isBookWidgetOpen = false;
	SetInputModetoGameandUI(false);
	bShowMouseCursor = false;
}
void APlayController::AddCurrentInstruction(FName aCurrentInstructionID)
{
	APlayGameMode* PlayGameMode = Cast<APlayGameMode>(GetWorld()->GetAuthGameMode());
	UDataTable* ItemTable = PlayGameMode->GetInstructionDB();
	if (ItemTable!=nullptr)
	{
		// find inventory item. 
		FNotes* NoteToAdd = ItemTable->FindRow<FNotes>(aCurrentInstructionID, "");
		CurrentInstruction = *NoteToAdd;
	}	
}

void APlayController::AddKeytoPossesion(FKeyData aKey)
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::FromInt(CurrentPossesedKeys.Num()));

	CurrentPossesedKeys.Add(aKey);
	CurrentPossesedKey = aKey;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::FromInt(CurrentPossesedKeys.Num()));

}


void APlayController::AddBatteryToPossesion(FBatteryData aBattery)
{
	CurrentPossesedBattery = aBattery;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::FromInt(CurrentPossesedKeys.Num()));

}
void APlayController::RemoveKeyfromPossesion(FKeyData aKey)
{
	for (int32 i = 0; i < CurrentPossesedKeys.Num(); i++)
	{
		if (CurrentPossesedKeys[i].KeyID == aKey.KeyID)
		{
			CurrentPossesedKeys.RemoveAt(i);
		}
	}
}

void APlayController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Use", IE_Pressed, this, &APlayController::Interact);
}
