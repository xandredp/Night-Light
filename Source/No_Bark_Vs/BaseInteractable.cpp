// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "BaseInteractable.h"


// Sets default values
ABaseInteractable::ABaseInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	PickupMesh->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	PickupMesh->SetupAttachment(RootComponent);

	PickupCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupCollisionComp"));
	PickupCollisionComp->SetupAttachment(PickupMesh);


	Name = "Name not set";
	Action = "Interact";

}

// Called when the game starts or when spawned
void ABaseInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseInteractable::Interact(APlayerController * playerController)
{

}

FString ABaseInteractable::GetInteractText() const
{
	return FString::Printf(TEXT("%s : Press E to %s"), *Name, *Action);
}
