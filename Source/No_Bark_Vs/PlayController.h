// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "NBCharacter.h"
#include "TypeClass.h"
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

	/************************************************************************/
	/* Interaction With key press                                                      */
	/************************************************************************/
	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void OpenInventory();

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void OpenMap();

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void Interact();
	
	// inventory interaction functions
	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void UseItem(FCurrentInventoryItemInfo iItemInfo);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void UnUseItem(FCurrentInventoryItemInfo iItemInfo);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void ReArrangeItems();


	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void DropItem(FCurrentInventoryItemInfo iItemInfo);

	/************************************************************************/
	/* Inventory                                                      */
	/************************************************************************/
	//Array of inventory
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TArray<FCurrentInventoryItemInfo> FCurrentInventory;

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void AddItemtoInventoryByID(FName ID, int ItemCurrentStackNumber);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void MoveItemWithinInventory(int ItemAIndex, int ItemBIndex);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void RemoveItemFromInventory(FCurrentInventoryItemInfo ItemToRemove);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void CraftItem(FInventoryItem ItemA, FInventoryItem ItemB, APlayController* PlayController);
	
	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void CloseInventory();
	
	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void SetInputModetoGameandUI(bool bHideCursor);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void ChangeMaxInventorySize(int iNoInventory);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		bool GetIsInventoryFull();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		int MaxInventorySize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		int32 LastAddedInventoryIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		bool bIsInventoryFull;
	
	/************************************************************************/
	/* Equipment                                                      */
	/************************************************************************/
	//Array of inventory
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TArray<FCurrentInventoryItemInfo> FCurrentEquippedWeapons;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		FCurrentInventoryItemInfo FCurrentEquippedMeleeWeapon;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TArray<FCurrentInventoryItemInfo> FCurrentEquipment;

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void AttachEquipmenttoCharacter(FCurrentInventoryItemInfo ItemToAttech);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void DetachEquipmentfromCharacter(FCurrentInventoryItemInfo ItemToDetach);
	
	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void MoveItemToInventory(FCurrentInventoryItemInfo iItemFromEqupment, int ItemBIndex);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void AddItemtoEquipmentByItem(FCurrentInventoryItemInfo iItemtoAdd, int toIndex);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void RemoveItemFromEquipment(FCurrentInventoryItemInfo ItemtoRemove);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void AttachEquipment(FCurrentInventoryItemInfo iItemInfo);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void UnAttachEquipment(FCurrentInventoryItemInfo iItemInfo);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		int MaxEquipmentSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		FCurrentInventoryItemInfo iItemRemovedfromEquipment;
	
	/* Class to add to SpawnLocation when picked up */
	UPROPERTY(EditDefaultsOnly, Category = "WeaponClass")
		TSubclassOf<ABaseWeapon> WeaponClass;

	/************************************************************************/
	/* Widgets                                                      */
	/************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		bool isMyInventoryOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		bool isMyMapOpen;

	UFUNCTION(BlueprintImplementableEvent)
		void ReloadInventory();

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		TSubclassOf<class UUserWidget> wInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		TSubclassOf<class UUserWidget> wEquipment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		TSubclassOf<class UUserWidget> wMinimap;
	// Variable to hold the widget After Creating it.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		UUserWidget* MyInventoryWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		UUserWidget* MyEquipmentWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		UUserWidget* MyMapWidget;

protected:

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		virtual void SetupInputComponent() override;
};
