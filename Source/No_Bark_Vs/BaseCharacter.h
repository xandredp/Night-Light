// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class NO_BARK_VS_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Character")
		float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Base Character")
		bool bisDead;

	//calculate health function
	UFUNCTION(BlueprintCallable, Category = "Config")
		virtual void CalculateHealth(float delta);

	// calculate death function
	virtual void CalculateDead();
	
};
