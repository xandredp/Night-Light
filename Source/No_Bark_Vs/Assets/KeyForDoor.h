// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core/BaseInteractable.h"
#include "KeyForDoor.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FKeyData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		FName KeyID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		UTexture2D* SplashArt;
	//Interactable DoorID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		FName DoorID;
};


UCLASS()
class NO_BARK_VS_API AKeyForDoor : public ABaseInteractable
{
	GENERATED_BODY()
	
public:
	AKeyForDoor();

	// Called when the game starts or when spawned
	virtual void Interact(APlayerController* playerController) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DestroyItemOnGround();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Config)
		FKeyData KeyConfig;

	//ItemID is same to KeyID. Edit Key ID plz. 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
		FName ItemID;
};
