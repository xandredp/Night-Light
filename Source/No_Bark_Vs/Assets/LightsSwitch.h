// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Assets/Actor_Light.h"
#include "Core/BaseInteractable.h"
#include "LightsSwitch.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API ALightsSwitch : public ABaseInteractable
{
	GENERATED_BODY()
	

public:
	ALightsSwitch();

	// Called when the game starts or when spawned
	virtual void Interact(APlayerController* playerController) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	////
	//// Called every frame
	//virtual void Tick(float DeltaSeconds) override;

	void DestroyItemOnGround();

	UFUNCTION(BlueprintCallable, Category = "Config")
		void ToggleSwitchOnAndOff();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Action")
		void InteractionFeedBack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		AActor_Light *LightActorComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		TArray<AActor_Light*> LightActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		bool IsSwitchOn;

	/*this bool variable checkes if door is interactable or not. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		bool LockSwitchForSpecificMechanic;
};
