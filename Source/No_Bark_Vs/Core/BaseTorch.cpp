// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseTorch.h"
#include "Core/No_Bark_Vs.h"
#include "Player/NBCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SpotLightComponent.h"



// Sets default values
ABaseTorch::ABaseTorch()
{
	TorchMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("TorchMesh"));
	TorchMesh->SetupAttachment(RootComponent);
	TorchMesh->SetVisibility(true);
	TorchMesh->bReceivesDecals = true;
	TorchMesh->CastShadow = true;
	TorchMesh->SetCollisionObjectType(ECC_WorldDynamic);
	TorchMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	TorchMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	TorchMesh->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TorchSpotlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("WeaponSpotlight"));
	TorchSpotlight->SetupAttachment(TorchMesh);
	TorchSpotlight->SetVisibility(true);
	TorchSpotlight->SetRelativeRotation(FRotator(0, 0, 0));
	TorchSpotlight->SetRelativeLocation(FVector(0, 0, -0));

	TorchSpotlight->SetIntensity(8000);
	TorchSpotlight->SetAttenuationRadius(16000);
	TorchSpotlight->SetOuterConeAngle(10);

}

// Called when the game starts or when spawned
void ABaseTorch::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseTorch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseTorch::SetOwningPawn(class ANBCharacter* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		Instigator = NewOwner;
		MyPawn = NewOwner;
	}
}
void ABaseTorch::ActivateTorch()
{
}

void ABaseTorch::DrainTorch()
{
}

void ABaseTorch::TorchCrank()
{
}

void ABaseTorch::SetTorchIntensity(float charge)
{
}


