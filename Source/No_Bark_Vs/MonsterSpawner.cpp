// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "MonsterSpawner.h"


// Sets default values
//AMonsterSpawner::AMonsterSpawner()
//{
// 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = false;
//
//	// populate array of spawn points
//	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonsterSpawnTargetPoint::StaticClass(), SpawnPoints);
//}
//
//// Called when the game starts or when spawned
//void AMonsterSpawner::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void AMonsterSpawner::Tick( float DeltaTime )
//{
//	Super::Tick( DeltaTime );
//
//}
//
//void AMonsterSpawner::SpawnMonster()
//{
//	// Get available spawn points
//	TArray<AActor*> AvailableSpawnPoints = GetAvailableSpawnPoints();
//	// Assign a random index
//	int32 RandomIndex = FMath::RandRange(0, AvailableSpawnPoints.Num());
//	// Assign random index as the current spawn point 
//	AMonsterSpawnTargetPoint* SpawnPoint = Cast<AMonsterSpawnTargetPoint>(AvailableSpawnPoints[RandomIndex]);
//
//	if (WhatToSpawn != NULL)
//	{
//		// Check for a valid world
//		UWorld* World = GetWorld();
//		if (World)
//		{
//			// Set the spawn params
//			FActorSpawnParameters SpawnParams;
//			SpawnParams.Owner = this;
//			SpawnParams.Instigator = Instigator;
//
//			// Get the spawn location
//			FVector SpawnLocation = SpawnPoint->GetActorLocation();
//
//			// Get the spawn rotation
//			FRotator SpawnRotation = SpawnPoint->GetActorRotation();
//
//			World->SpawnActor<ASkinnyMonster>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
//		}
//	}
//}