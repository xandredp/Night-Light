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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
		class USpotLightComponent* TorchSpotlight;

	//gain Health
	FTimerHandle StartReducingEnergyTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		float StunIntensity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		float MaxIntensity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		float MaxEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		float CurrentEnergy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		float EnergyReductionRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		float ReductionTimerRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		float MaxUseDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		float CurrentUseDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		float EnergyReductionOnPowerUse;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		float EnergyIncreaseOnCrank;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Owning")
		class ANBCharacter* MyPawn;


	/* Set the Torch's owning pawn */
	void SetOwningPawn(class ANBCharacter* NewOwner);


	UFUNCTION(BlueprintCallable, Category = "Torch")
		void DrainTorchEnergy();
	


	UFUNCTION(BlueprintCallable, Category = "Torch")
		void DecreaseEnergy();

	UFUNCTION(BlueprintCallable, Category = "Torch")
		void IncreaseEnergy();

	UFUNCTION(BlueprintCallable, Category = "Torch")
		void ActivateTorch();

	UFUNCTION(BlueprintCallable, Category = "Torch")
		void TorchCrank();

	UFUNCTION(BlueprintCallable, Category = "Torch")
		void TorchOnOff(bool bOnOrOff);

	UFUNCTION(BlueprintCallable, Category = "Torch")
		void SetTorchIntensity();

protected:
	FHitResult TorchLightTrace(const FVector &TraceFrom, const FVector &TraceTo) const;
	void ProcessInstantHit(const FHitResult &Impact);





};