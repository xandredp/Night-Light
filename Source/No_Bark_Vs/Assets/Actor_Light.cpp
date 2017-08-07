// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/No_Bark_Vs.h"
#include "Actor_Light.h"


// Sets default values
AActor_Light::AActor_Light()
{
	LightStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	LightStaticMesh->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	LightStaticMesh->SetupAttachment(RootComponent);

	SpotLightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("PointLight"));
	SpotLightComp->SetupAttachment(LightStaticMesh);

	bIsLightOn = true;
	//this->SetActorHiddenInGame(bIsLightOn);
}

// Called when the game starts or when spawned
void AActor_Light::BeginPlay()
{
	Super::BeginPlay();
}
