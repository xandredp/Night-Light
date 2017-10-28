// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/SaveGame.h"

#include "Core/TypeClass.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

#include "SaveGameModule.generated.h" 


/**
 * 
 */

UCLASS()
class NO_BARK_VS_API USaveGameModule : public USaveGame
{
	GENERATED_BODY()
	
public:

	// Saved variables from NBCharacter

	UPROPERTY(EditAnywhere, Category = "Save")
		FVector PlayerPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		int CurrentScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float MaxHealth;


	// Weapon stuff 

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
		TSubclassOf <class ABaseWeapon> WeaponClass;
	//Hand Held Item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
	  bool HasWeapon;

	//Primary Weapon Held Item

	// Torch

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawn)
		TSubclassOf <class ABaseTorch> TorchClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		bool HasTorch;


	// This stuff belongs to the current weapon

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		int32 CurrentClip;
	//weapon reloaded ammo
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		int32 CurrentAmmo;

	UPROPERTY(EditAnywhere, Category = "Basic")
		FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
		uint32 UserIndex;

	USaveGameModule();
	
	UFUNCTION(BlueprintCallable, Category = "Config")
		void SaveGame(ACharacter * ThisCharacter);

	UFUNCTION(BlueprintCallable, Category = "Config")
		void LoadGame(ACharacter * ThisCharacter);
	
};
