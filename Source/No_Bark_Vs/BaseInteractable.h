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
		UStaticMeshComponent* PickupMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
		class UBoxComponent* PickupCollisionComp;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UFUNCTION(BlueprintImplementableEvent)
	virtual void Interact(APlayerController* playerController);

	UFUNCTION(BlueprintCallable, Category = "Pickup")
		FString GetInteractText() const;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
		FString Name;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
		FString Action;
};
