// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryPickup.h"
#include "Core/No_Bark_Vs.h"
#include "Player/PlayController.h"
#include "Player/NBCharacter.h"





ABatteryPickup::ABatteryPickup(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemID = FName("Please EnterID");
}

void ABatteryPickup::Interact(APlayerController* playerController)
{
	APlayController* aPlayController = Cast<APlayController>(playerController);
	if (aPlayController)
	{
		if (SoundPickUp != nullptr)
		{
			PlaySound(SoundPickUp);
		}
		//PlayPickupAnimation
		ANBCharacter* MyPawn = Cast<ANBCharacter>(aPlayController->GetPawn());
		if (MyPawn)
		{
			MyPawn->PlayPickUpAnimation();
		}
		aPlayController->AddBatteryToPossesion(BatteryConfig);
		PlayPickupSound();
		DestroyItemOnGround();
	}
}

void ABatteryPickup::BeginPlay()
{
	Super::BeginPlay();

}

void ABatteryPickup::DestroyItemOnGround()
{
	Destroy();
}
