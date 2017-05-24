// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "SkinnyMonster.h"


ASkinnyMonster::ASkinnyMonster()
{
	MovementSpeed = 75.0f;
	Health = 100.0f;
	AttackDamage = 20.0f;
	AttackRange = 150.0f;

	AttackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Range Sphere"));
//	AttackRangeSphere->SetRelativeLocation(FVector(0, 0, 0));

	AttackRangeSphere->SetSphereRadius(AttackRange);
	AttackRangeSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackRangeSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackRangeSphere->SetupAttachment(GetCapsuleComponent());
	AttackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ASkinnyMonster::OnOverlapWithCharacter);
	//AttackRangeSphere->IsVisible = true;
	//AttackRangeSphere->OnComponentEndOverlap.AddDynamic(this, &ASkinnyMonster::OnEndOverlapWithCharacter);
}


void ASkinnyMonster::OnOverlapWithCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Other Actor is the actor that triggered the event.Check that is not ourself.
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		USkinnyMonsterAnimInstance* AnimInstance = Cast<USkinnyMonsterAnimInstance>(GetMesh()->GetAnimInstance());
		
		if (AnimInstance)
		{	
			// Set CanAttack in AnimInstance to true
			AnimInstance->CanAttack = true;
		}
	}
}

//void ASkinnyMonster::OnEndOverlapWithCharacter(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
//	{
//		USkinnyMonsterAnimInstance* AnimInstance = Cast<USkinnyMonsterAnimInstance>(GetMesh()->GetAnimInstance());
//
//		if (AnimInstance)
//		{
//			// Set CanAttack in AnimInstance to false
//			AnimInstance->CanAttack = false;
//		}
//	}
//}