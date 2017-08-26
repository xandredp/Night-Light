// Fill out your copyright notice in the Description page of Project Settings.
#include "BaseInteractable.h"
#include "Core/No_Bark_Vs.h"



// Sets default values
ABaseInteractable::ABaseInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>("PickupSkeletalMesh");
	PickupSkeletalMesh->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	//PickupSkeletalMesh->SetupAttachment(RootComponent);

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	//PickupMesh->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	PickupMesh->SetupAttachment(PickupSkeletalMesh);

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
	FString RetVal;

	if ((Name > "") && (Action > ""))
	{
		//RetVal = FString::Printf(TEXT("%s : Press E to %s"), *Name, *Action);
		RetVal = FString::Printf(TEXT("%s"), *Action);
	}
	else
	{
		RetVal = "";
	}
	return RetVal;
}
