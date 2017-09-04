// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseTorch.generated.h"

UCLASS()
class NO_BARK_VS_API ABaseTorch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseTorch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//// Called every frame
	//virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
		class USpotLightComponent* TorchSpotlight;

	//gain Health
	FTimerHandle StartReducingEnergyTimerHandle;


	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
		float MaxEnergy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
		float CurrentEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float EnergyReductionRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status)
		float ReductionTimerRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
		float MaxUseDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
		float CurrentUseDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
		float EnergyReductionOnPowerUse;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
		float EnergyIncreaseOnCrank;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Owning")
		class ANBCharacter* MyPawn;


	/* Set the Torch's owning pawn */
	void SetOwningPawn(class ANBCharacter* NewOwner);


	UFUNCTION(BlueprintCallable, Category = "Torch")
		void DrainTorchEnergy();
	
	UFUNCTION(BlueprintCallable, Category = "Torch")
		void ActivateTorch();

	UFUNCTION(BlueprintCallable, Category = "Condition")
		void DecreaseEnergy();

	UFUNCTION(BlueprintCallable, Category = "Condition")
		void IncreaseEnergy();

	UFUNCTION(BlueprintCallable, Category = "Torch")
		void TorchCrank();

	UFUNCTION(BlueprintCallable, Category = "Torch")
		void TorchOnOff(bool bOnOrOff);

	UFUNCTION(BlueprintCallable, Category = "Torch")
		void SetTorchIntensity();



};
