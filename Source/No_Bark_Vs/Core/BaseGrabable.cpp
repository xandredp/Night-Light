// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseGrabable.h"


// Sets default values
ABaseGrabable::ABaseGrabable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseGrabable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseGrabable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

