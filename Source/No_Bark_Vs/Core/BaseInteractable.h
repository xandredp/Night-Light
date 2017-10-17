// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseInteractable.generated.h"

UCLASS()
class NO_BARK_VS_API ABaseInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseInteractable();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colllision")
		USkeletalMeshComponent* PickupSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colllision")
		UStaticMeshComponent* PickupMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
		class UBoxComponent* PickupCollisionComp;

	/* Plays the pickup sound*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Sound")
		UAudioComponent* AudioLoopComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
		USoundBase* SoundPickUp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UFUNCTION(BlueprintImplementableEvent)
	virtual void Interact(APlayerController* playerController);
	
	//by putting blueprintImplementableEvent gives freedom to non coding designers
	UFUNCTION(BlueprintImplementableEvent)
	void InteractEvent();

	UFUNCTION(BlueprintCallable, Category = "Pickup")
		FString GetInteractText() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
		FString Action;

protected:
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void PlaySound(USoundBase* CueToPlay);
};
