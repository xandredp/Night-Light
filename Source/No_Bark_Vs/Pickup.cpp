// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "Pickup.h"

APickup::APickup()
{
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	ItemID = FName("Please EnterID");

}