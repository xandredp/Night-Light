// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "Player/NBCharacter.h"
#include "Core/TypeClass.h"
#include "Assets/KeyForDoor.h"
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
		void Interact();

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		bool IsInteract();

	/************************************************************************/
	/* Inventory                                                      */
	/************************************************************************/

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void SetInputModetoGameandUI(bool bHideCursor);
		
	/* Class to add to SpawnLocation when picked up */
	UPROPERTY(EditDefaultsOnly, Category = "WeaponClass")
		TSubclassOf<ABaseWeapon> WeaponClass;

	/************************************************************************/
	/* Widgets                                                      */
	/************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		bool isBookWidgetOpen;

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void OpenBookWidget();

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void CloseBookWidget();

	// Reference UMG Asset in the Editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		TSubclassOf<class UUserWidget> wBook;

	// Variable to hold the widget After Creating it.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		UUserWidget* wBookWidget;

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void AddCurrentInstruction(FName aCurrentInstructionID);

	//The interactable that player is currently looking at. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FNotes CurrentInstruction;
	/************************************************************************/
	/* Keys                                                      */
	/************************************************************************/

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		TArray<FKeyData> CurrentPossesedKeys;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		FKeyData CurrentPossesedKey;

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void AddKeytoPossesion(FKeyData aKey);

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void RemoveKeyfromPossesion(FKeyData aKey);


	/************************************************************************/
	/* Game Currency                                                      */
	/************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		int MyCurrentCurrency;
protected:

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		virtual void SetupInputComponent() override;
};
