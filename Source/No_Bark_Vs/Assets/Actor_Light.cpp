// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "Actor_Light.h"


// Sets default values
AActor_Light::AActor_Light()
{
	PointLightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	this->SetActorHiddenInGame(false);
}

// Called when the game starts or when spawned
void AActor_Light::BeginPlay()
{
	Super::BeginPlay();
}

