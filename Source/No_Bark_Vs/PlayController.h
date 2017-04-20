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

		APlayController();
public:
	//The interactable that player is currently looking at. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ABaseInteractable* CurrentInteractable;

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void Interact();
	
	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void AddItemtoInventoryByID(FName ID);

	//Array of inventory
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TArray<FInventoryItem> Inventory;

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void OpenInventory();

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		TSubclassOf<class UUserWidget> wInventory;

	// Variable to hold the widget After Creating it.
	UUserWidget* MyInventory;

protected:

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		virtual void SetupInputComponent() override;
};
