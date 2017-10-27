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

	UPROPERTY(EditAnywhere, Category = "Save")
		FVector PlayerPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Save")
		float Health;

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
