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
	
	FTimerHandle StartLightingTimerHandle;
	FTimerHandle StopLightingTimerHandle;


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
	//Energy decreased everysecond
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		float DeductEnergyBy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		bool bIsEnergyZero;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		bool bIsEnergyMaxed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float EnergyTimerRate;


	//Energy decreased everysecond using set energy amt 
	UFUNCTION(BlueprintCallable, Category = "Config")
		void DecreaseEnergy();
	//Energy Added on Everny Cracking action using set energy amt 
	UFUNCTION(BlueprintCallable, Category = "Config")
		void IncreaseEnergy();

	UFUNCTION(BlueprintCallable, Category = "Config")
		void DecreaseEnergyByFloat(float deductBy);
	//Energy Added on Everny Cracking action
	UFUNCTION(BlueprintCallable, Category = "Config")
		void IncreaseEnergyByFloat(float addBy);
	//Check bIsEnergyZero
	UFUNCTION(BlueprintCallable, Category = "Config")
		bool CheckbIsEnergyZero();

	UFUNCTION(BlueprintCallable, Category = "Config")
		bool CheckbIsEnergyMaxed();

	UFUNCTION(BlueprintCallable, Category = "Config")
		void LightingOnStart();

	UFUNCTION(BlueprintCallable, Category = "Config")
		void LightingOnStop();

};
