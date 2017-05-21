// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseInteractable.h"
#include "CrackingObjForLight.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API ACrackingObjForLight : public ABaseInteractable
{
	GENERATED_BODY()
	
public:
	ACrackingObjForLight();
	
	// Called when the game starts or when spawned
	virtual void Interact(APlayerController* playerController) override;

	void DestroyItemOnGround();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		float MaxEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		float Energy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		float AddEnergyBy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		float DeductEnergyBy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		bool IsLightOn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		bool IsEnergyZero;
	
};
