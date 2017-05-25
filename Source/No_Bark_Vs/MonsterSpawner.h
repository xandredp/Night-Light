// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Monster.h"
#include "SkinnyMonster.h"
#include "MonsterSpawnTargetPoint.h"
#include "GameFramework/Actor.h"
#include "MonsterSpawner.generated.h"

UCLASS()
class NO_BARK_VS_API AMonsterSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMonsterSpawner();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void SpawnMonster();

	FORCEINLINE TArray<AActor*> GetAvailableSpawnPoints() { return SpawnPoints; }

protected:
	TArray<AActor*> SpawnPoints;

	UPROPERTY(EditAnywhere, Category = "Spawning")
		AMonsterSpawnTargetPoint* SpawnPoint;

	// The actor to spawn
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class AMonster> WhatToSpawn;

	UInputComponent* Input;
};
