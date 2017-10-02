// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Monsters/Base/NBBaseAI.h"
#include "NBSkinnyAI.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API ANBSkinnyAI : public ANBBaseAI
{
	GENERATED_BODY()
	
	
public:
	ANBSkinnyAI();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//void Tick(float DeltaSeconds) override;

	
};
