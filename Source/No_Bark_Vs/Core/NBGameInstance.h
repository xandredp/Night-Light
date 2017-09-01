// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/GameInstance.h"
#include "NBGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API UNBGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UNBGameInstance(const FObjectInitializer& ObjectInitializer);

	/** Increment this value in any map, change map, and notice it persists! */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ThisLevel)
		int32 LevelValue;
	
	
};
