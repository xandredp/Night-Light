// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/BaseInteractable.h"
#include "WeaponPickUp.generated.h"

/**
 * 
 */
UCLASS()
class NO_BARK_VS_API AWeaponPickUp : public ABaseInteractable
{
	GENERATED_BODY()
	
	
	
public:
	AWeaponPickUp();

	// Called when the game starts or when spawned
	virtual void Interact(APlayerController* playerController) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DestroyItemOnGround();

	//ItemID is same to KeyID. Edit Key ID plz. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		FName ItemID;
};
