// Fill out your copyright notice in the Description page of Project Settings.

#include "MonsterAtttackComponent.h"
 #include "Core/No_Bark_Vs.h"

 
 
 // Sets default values
 AMonsterAtttackComponent::AMonsterAtttackComponent()
 {
  	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
 	PrimaryActorTick.bCanEverTick = true;
 
 }
 
 // Called when the game starts or when spawned
 void AMonsterAtttackComponent::BeginPlay()
 {
 	Super::BeginPlay();
 	
 }
 
 // Called every frame
 void AMonsterAtttackComponent::Tick( float DeltaTime )
 {
 	Super::Tick( DeltaTime );
 
 }
 
