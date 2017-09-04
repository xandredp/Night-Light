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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
		class USpotLightComponent* TorchSpotlight;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
		class USkeletalMeshComponent* TorchMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
		float MaxUseDistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Config")
		float CurrentUseDistance;

	UFUNCTION(BlueprintCallable, Category = "Torch")
		void ActivateTorch();

	UFUNCTION(BlueprintCallable, Category = "Torch")
		void DrainTorch();

	UFUNCTION(BlueprintCallable, Category = "Torch")
		void TorchCrank();

	void SetTorchIntensity(float charge);

	/* Set the Torch's owning pawn */
	void SetOwningPawn(class ANBCharacter* NewOwner);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Owning")
		class ANBCharacter* MyPawn;


};
