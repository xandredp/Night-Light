// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BaseInteractable.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API APickup : public ABaseInteractable
{
	GENERATED_BODY()
	
	
public:
	APickup();

	// Called when the game starts or when spawned
	virtual void Interact(APlayerController* playerController) override;
	
	void DestroyItemOnGround();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Colllision")
		UStaticMeshComponent* PickupMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
		class USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
		class UBoxComponent* WeaponCollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Collision")
		class UBoxComponent* PickupCollisionComp;


	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Pickup")
		void OnUsed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pickup")
	FName ItemID;
	
};
