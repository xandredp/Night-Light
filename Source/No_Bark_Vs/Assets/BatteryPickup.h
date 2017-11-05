// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core/BaseInteractable.h"
#include "BatteryPickup.generated.h"

/**
 * 
 */


USTRUCT(BlueprintType)
struct FBatteryData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		FName BatteryID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		UTexture2D* SplashArt;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		float ChargeAmount;

};


UCLASS(ABSTRACT)
class NO_BARK_VS_API ABatteryPickup : public ABaseInteractable
{
	GENERATED_BODY()
	
protected:
	ABatteryPickup(const FObjectInitializer& ObjectInitializer);
public:

	
	virtual void Interact(APlayerController* playerController) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
		void PlayPickupSound();

	UFUNCTION(BlueprintCallable)
		void DestroyItemOnGround();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		FBatteryData BatteryConfig;

	//Interactable DoorID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		FName ItemID;

};
