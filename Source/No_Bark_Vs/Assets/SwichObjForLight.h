// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseInteractable.h"

#include "SwichObjForLight.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API ASwichObjForLight : public ABaseInteractable
{
	GENERATED_BODY()
	
public:
	ASwichObjForLight();

	// Called when the game starts or when spawned
	virtual void Interact(APlayerController* playerController) override;

	void DestroyItemOnGround();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		ACrackingObjForLight *aCrackingObjForLight;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		bool bIsLightOn;

	
};
