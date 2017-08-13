// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/No_Bark_Vs.h"
#include "Player/PlayController.h"
#include "AmmoPickUp.h"





AAmmoPickUp::AAmmoPickUp()
{
	ItemID = FName("Please EnterID");
	ClipsInBox = 5;
}

void AAmmoPickUp::Interact(APlayerController* playerController)
{
	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{
		//PlayPickupAnimation
		ANBCharacter* MyPawn = Cast<ANBCharacter>(aPlayController->GetPawn());
		if (MyPawn)
		{
			if (MyPawn->PickingUpAnimation != NULL)
			{
				MyPawn->ArmAnimInstance = MyPawn->FPSCharacterArmMesh->GetAnimInstance();
				if (MyPawn->ArmAnimInstance != NULL)
				{
					MyPawn->ArmAnimInstance->Montage_Play(MyPawn->PickingUpAnimation, 1.0f);
				}
			}

		}
		PickupClips();
	}
}

void AAmmoPickUp::BeginPlay()
{
	Super::BeginPlay();

}

void AAmmoPickUp::DestroyItemOnGround()
{
	Destroy();
}
