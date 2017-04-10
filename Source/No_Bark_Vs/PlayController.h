// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "PlayController.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API APlayController : public APlayerController
{
	GENERATED_BODY()

public:
	//The interactable that player is currently looking at. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ABaseInteractable* CurrentInteractable;

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		void Interact();

protected:

	UFUNCTION(BlueprintCallable, Category = "Interactable")
		virtual void SetupInputComponent() override;
};
