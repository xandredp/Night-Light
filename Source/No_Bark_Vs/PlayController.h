// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "NBCharacter.h"
#include "PlayController.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API APlayController : public APlayerController
{
	GENERATED_BODY()
public:
	APlayController();


	//The interactable that player is currently looking at. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ABaseInteractable* CurrentInteractable;

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void Interact();
	
	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void AddItemtoInventoryByID(FName ID);
	// inventory interaction functions
	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void UseItem(FCurrentInventoryItemInfo iItemInfo);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void UnUseItem(FCurrentInventoryItemInfo iItemInfo);
	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void DropItem(FCurrentInventoryItemInfo iItemInfo);
	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void EnableActionBar(FCurrentInventoryItemInfo iItemInfo);

	//Array of inventory
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TArray<FCurrentInventoryItemInfo> FCurrentInventory;

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void OpenInventory();

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void CloseInventory();

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void CraftItem(FInventoryItem ItemA, FInventoryItem ItemB, APlayController* PlayController);

	UFUNCTION(BlueprintImplementableEvent)
		void ReloadInventory();

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void SetInputModetoGameandUI(bool bHideCursor);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void ChangeMaxInventorySize(int iNoInventory);

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		TSubclassOf<class UUserWidget> wInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		TSubclassOf<class UUserWidget> wEquipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		bool isMyInventoryOpen;

	// Variable to hold the widget After Creating it.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
	UUserWidget* MyInventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		UUserWidget* MyEquipmentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		int MaxInventorySize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		int32 LastAddedInventoryIndex;
protected:

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		virtual void SetupInputComponent() override;
};
