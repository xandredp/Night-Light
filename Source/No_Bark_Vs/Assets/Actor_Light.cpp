// Fill out your copyright notice in the Description page of Project Settings.
#include "Actor_Light.h"
#include "Core/No_Bark_Vs.h"
#include "../Monsters/Base/NBBaseAI.h"



// Sets default values
AActor_Light::AActor_Light()
{
	LightStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("PickupMesh");
	LightStaticMesh->SetRelativeLocation(FVector(0.0, 0.0, 0.0));
	//LightStaticMesh->SetupAttachment(RootComponent);
	//SetRootComponent(LightStaticMesh);

	SpotLightComp = CreateDefaultSubobject<USpotLightComponent>(TEXT("PointLight"));
	SpotLightComp->SetupAttachment(LightStaticMesh);


	// This sphere component is attached to the hand to detect a hit
	SphereComp0 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp0"));
	SphereComp0->SetSphereRadius(100);
	SphereComp0->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp0->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp0->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp0->SetupAttachment(LightStaticMesh);
	SphereComp0->OnComponentBeginOverlap.AddDynamic(this, &AActor_Light::OnOverlapChangeCharacterStatus);
	SphereComp0->OnComponentEndOverlap.AddDynamic(this, &AActor_Light::OnEndOverlapChangeCharacterStatus);

	// This sphere component is attached to the hand to detect a hit
	SphereComp1 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp1"));
	SphereComp1->SetSphereRadius(100);
	SphereComp1->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp1->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp1->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp1->SetupAttachment(LightStaticMesh);
	SphereComp1->OnComponentBeginOverlap.AddDynamic(this, &AActor_Light::OnOverlapChangeCharacterStatus);


	// This sphere component is attached to the hand to detect a hit
	SphereComp2 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp2"));
	SphereComp2->SetSphereRadius(100);
	SphereComp2->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp2->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp2->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp2->SetupAttachment(LightStaticMesh);
	SphereComp2->OnComponentBeginOverlap.AddDynamic(this, &AActor_Light::OnOverlapChangeCharacterStatus);



	// This sphere component surrounds the monster to determine attack range
	SphereComp3 = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp3"));
	SphereComp3->SetSphereRadius(150);
	SphereComp3->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp3->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp3->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp3->SetupAttachment(LightStaticMesh);
	SphereComp3->OnComponentBeginOverlap.AddDynamic(this, &AActor_Light::OnOverlapChangeCharacterStatus);

	bIsLightOn = true;
	//this->SetActorHiddenInGame(bIsLightOn);
}

void AActor_Light::OnOverlapChangeCharacterStatus(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		ANBBaseAI * BaseAI = Cast<ANBBaseAI>(OtherActor);
		if (BaseAI)
		{
			if (SpotLightComp->IsVisible() == true)
			{
				BaseAI->bisMonsterInLight = true;
			}
			else
			{
				BaseAI->bisMonsterInLight = false;
			}

			BaseAI->MonsterInLight(BaseAI->bisMonsterInLight);
		}
	}
}

void AActor_Light::OnEndOverlapChangeCharacterStatus(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		ANBBaseAI * BaseAI = Cast<ANBBaseAI>(OtherActor);
		if (BaseAI)
		{
			BaseAI->bisMonsterInLight = false;

			BaseAI->MonsterInLight(BaseAI->bisMonsterInLight);
		}
	}
}

// Called when the game starts or when spawned
void AActor_Light::BeginPlay()
{
	Super::BeginPlay();
}
