// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Actor_Light.generated.h"

UCLASS()
class NO_BARK_VS_API AActor_Light : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActor_Light();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UPointLightComponent *PointLightComp;

	UFUNCTION(BlueprintImplementableEvent)
		void TogglePointLightComp(bool blightvisiblity);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		bool bIsLightOn;
};

