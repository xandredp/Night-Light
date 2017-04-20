// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "PlayController.h"
#include "Pickup.h"

APickup::APickup()
{

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetRelativeRotation(FRotator(0, 180, 0), false);
	WeaponMesh->SetupAttachment(RootComponent);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	PickupMesh->SetupAttachment(RootComponent);

	WeaponCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionComp"));
	WeaponCollisionComp->SetupAttachment(WeaponMesh);

	PickupCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupCollisionComp"));
	PickupCollisionComp->SetupAttachment(PickupMesh);


	ItemID = FName("Please EnterID");



}

void APickup::Interact(APlayerController* playerController)
{

//	Super::Interact(playerController);

	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Interactiion Successed");
		aPlayController->AddItemtoInventoryByID(ItemID);
		
	}

	DestroyItemOnGround();
}

void APickup::DestroyItemOnGround()
{
	Destroy();
}
