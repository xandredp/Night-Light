// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core/BaseInteractable.h"
#include "TorchAttachmentPickUp.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API ATorchAttachmentPickUp : public ABaseInteractable
{
	GENERATED_BODY()

public:
	ATorchAttachmentPickUp();

	// Called when the game starts or when spawned
	virtual void Interact(APlayerController* playerController) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DestroyItemOnGround();

	// function to turn torch on as torch gets picked up
	UFUNCTION(BlueprintImplementableEvent)
		void BeginWithTorchOn();

	//ItemID is same to KeyID. Edit Key ID plz. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		FName ItemID;

	//When This item is interacted door will close
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		class ADoor* DoorToInteract;
};
