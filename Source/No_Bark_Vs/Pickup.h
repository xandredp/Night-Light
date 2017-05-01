// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseInteractable.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API APickup : public ABaseInteractable
{
	GENERATED_BODY()
	
	
public:
	APickup();

	// Called when the game starts or when spawned
	virtual void Interact(APlayerController* playerController) override;
	
	void DestroyItemOnGround();


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Pickup")
		void OnUsed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	FName ItemID;
	
};
