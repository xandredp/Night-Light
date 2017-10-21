// Fill out your copyright notice in the Description page of Project Settings.
#include "Actor_Light.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SpotLightComponent.h"
#include "../Monsters/Base/NBBaseAI.h"
//nclude "Core/No_Bark_Vs.h"



// Sets default values
AActor_Light::AActor_Light()
{
	LightStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	LightStaticMesh->SetRelativeLocation(FVector(0.0, 0.0, 0.0));

	SpotLightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("PointLight"));
	SpotLightComp->SetupAttachment(LightStaticMesh);



	// This sphere component is attached to the hand to detect a hit
	SphereComp0 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp0"));
	SphereComp0->SetSphereRadius(300);
	SphereComp0->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp0->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp0->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp0->SetupAttachment(LightStaticMesh);
	SphereComp0->OnComponentBeginOverlap.AddDynamic(this, &AActor_Light::OnOverlapChangeAIStates);
	SphereComp0->OnComponentEndOverlap.AddDynamic(this, &AActor_Light::OnEndOverlapChangeAIStates);

	// This sphere component is attached to the hand to detect a hit
	SphereComp1 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp1"));
	SphereComp1->SetSphereRadius(60);
	SphereComp1->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp1->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp1->SetupAttachment(LightStaticMesh);
	SphereComp1->OnComponentBeginOverlap.AddDynamic(this, &AActor_Light::OnOverlapChangeAIStates);


	SphereComp2 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp2"));
	SphereComp2->SetSphereRadius(60);
	SphereComp2->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp2->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp2->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp2->SetupAttachment(LightStaticMesh);
	SphereComp2->OnComponentBeginOverlap.AddDynamic(this, &AActor_Light::OnOverlapChangeAIStates);
	
	SphereComp3 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp3"));
	SphereComp3->SetSphereRadius(60);
	SphereComp3->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp3->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp3->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp3->SetupAttachment(LightStaticMesh);
	SphereComp3->OnComponentBeginOverlap.AddDynamic(this, &AActor_Light::OnOverlapChangeAIStates);
	

	bIsLightOn = true;
}

// Called when the game starts or when spawned
void AActor_Light::BeginPlay()
{
	Super::BeginPlay();
}

void AActor_Light::OnOverlapChangeAIStates(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		ANBBaseAI* BaseAI = Cast<ANBBaseAI>(OtherActor);
		if (BaseAI)
		{
			if (SpotLightComp->IsVisible() == true)
			{
				BaseAI->bisMonsterInLight=true;
				BaseAI->MonsterInLight(true);
			}
			else
			{
				BaseAI->bisMonsterInLight = false;
				BaseAI->MonsterInLight(false);
			}
		
		}
	}
}

void AActor_Light::OnEndOverlapChangeAIStates(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		ANBBaseAI* BaseAI = Cast<ANBBaseAI>(OtherActor);
		if (BaseAI)
		{
			BaseAI->bisMonsterInLight = false;
			BaseAI->MonsterInLight(false);
		}
	}
}
