// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponPickUp.h"
#include "Player/PlayController.h"

AWeaponPickUp::AWeaponPickUp()
{
	ItemID = FName("Please EnterID");
}


void AWeaponPickUp::Interact(APlayerController * playerController)
{
	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{
		//PlayPickupAnimation
		ANBCharacter* MyPawn = Cast<ANBCharacter>(aPlayController->GetPawn());
		if (MyPawn)
		{
			if (MyPawn->CurrentWeapon == NULL)
			{
				MyPawn->PlayPickUpAnimation();
				MyPawn->EquipPrimaryWeapon();
			}

		}
		DestroyItemOnGround();
	}
}

void AWeaponPickUp::BeginPlay()
{
	Super::BeginPlay();

}

void AWeaponPickUp::DestroyItemOnGround()
{
	Destroy();
}