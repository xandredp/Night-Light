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
	
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	//	USphereComponent* NavMeshModifierSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		class UStaticMeshComponent* LightStaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USpotLightComponent *SpotLightComp;

	UFUNCTION(BlueprintImplementableEvent)
		void ToggleSpotLightComp(bool blightvisiblity);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightObj")
		bool bIsLightOn;

	// Sphere component around monster this triggers attack animation
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* SphereComp0;

	// Sphere component attached to the hand 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* SphereComp1;

	// Sphere component attached to the hand 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* SphereComp2;

	// Sphere component attached to the hand 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class USphereComponent* SphereComp3;

	UFUNCTION()
		void OnOverlapChangeAIStates(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlapChangeAIStates(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};

