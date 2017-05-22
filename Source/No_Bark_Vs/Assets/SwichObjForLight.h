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

	UFUNCTION(BlueprintCallable, Category = "Config")
		void SwitchingOnAndOff();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		ACrackingObjForLight *aCrackingObjForLight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		AActor_Light *LightActorComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		bool bIsLightOn;

	
};
