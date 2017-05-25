// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "Actor_Light.h"


// Sets default values
AActor_Light::AActor_Light()
{
	NavMeshModifierSphere = CreateDefaultSubobject<USphereComponent>(TEXT("NavMesh Modifier Sphere"));
	PointLightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));

	RootComponent = NavMeshModifierSphere;
	NavMeshModifierSphere->SetSphereRadius(200.0f);
	//NavMeshModifierSphere->SetRelativeLocation(PointLightComp->RelativeLocation - FVector(0.0f, 20.0f, 0.0f));
	
	bIsLightOn = false;
	this->SetActorHiddenInGame(bIsLightOn);
}

// Called when the game starts or when spawned
void AActor_Light::BeginPlay()
{
	Super::BeginPlay();
}
