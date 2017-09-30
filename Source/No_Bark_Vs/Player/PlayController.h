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

	/************************************************************************/
	/* Inventory                                                      */
	/************************************************************************/
	
	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void SetInputModetoGameandUI(bool bHideCursor);
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		int MaxEquipmentSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		FCurrentInventoryItemInfo iItemRemovedfromEquipment;
	
	/* Class to add to SpawnLocation when picked up */
	UPROPERTY(EditDefaultsOnly, Category = "WeaponClass")
		TSubclassOf<ABaseWeapon> WeaponClass;

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
	/* TorchPossesion                                                      */
	/************************************************************************/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		bool IsTorchOn;
	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void SetIsTorchOn(bool istorchonValue);


	/************************************************************************/
	/* Game Currency                                                      */
	/************************************************************************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets)
		int MyCurrentCurrency;
protected:

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		virtual void SetupInputComponent() override;
};
