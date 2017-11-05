// Fill out your copyright notice in the Description page of Project Settings.
#include "BaseInteractable.h"
#include "Core/No_Bark_Vs.h"
#include "Components/AudioComponent.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Sound/SoundCue.h"
#include "Components/BoxComponent.h"

// Sets default values

ABaseInteractable::ABaseInteractable(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	PickupSkeletalMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("PickupSkeletalMesh"));
	PickupSkeletalMesh->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	RootComponent = PickupSkeletalMesh;//PickupSkeletalMesh->SetupAttachment(RootComponent);
	//SetRootComponent(PickupSkeletalMesh);
	PickupMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("PickupMesh"));
	//PickupMesh->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	PickupMesh->SetupAttachment(PickupSkeletalMesh);

 	AudioLoopComp = ObjectInitializer.CreateDefaultSubobject<UAudioComponent>(this, TEXT("PickUpSound"));
 	AudioLoopComp->bAutoActivate = false;
 	AudioLoopComp->bAutoDestroy = false;
 	AudioLoopComp->bStopWhenOwnerDestroyed = false;
 	AudioLoopComp->SetupAttachment(PickupMesh);
 	AudioLoopComp->SetSound(SoundPickUp);

	PickupCollisionComp = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("PickupCollisionComp"));
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
	InteractEvent();
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

void ABaseInteractable::PlaySound(USoundBase * CueToPlay)
{
	AudioLoopComp->SetSound(CueToPlay);
	AudioLoopComp->Play();
}
