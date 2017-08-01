// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core/BaseInteractable.h"
#include "AmmoPickUp.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API AAmmoPickUp : public ABaseInteractable
{
	GENERATED_BODY()
	
public:
	AAmmoPickUp();

	// Called when the game starts or when spawned
	virtual void Interact(APlayerController* playerController) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DestroyItemOnGround();

	UFUNCTION(BlueprintImplementableEvent)
		void PickupClips();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		int32 ClipsInBox;
};
