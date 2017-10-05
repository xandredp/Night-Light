// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "Core/TypeClass.h"
#include "PlayGameMode.generated.h"

/**
 * 
 */
//

UCLASS()
class NO_BARK_VS_API APlayGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	APlayGameMode();
	class UDataTable* GetItemDB() const { return ItemDB; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		EGameModeSoundType GameModeState;

protected:

	UPROPERTY(EditDefaultsOnly)
		class UDataTable* ItemDB;

};
