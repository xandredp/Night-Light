// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "NBCharacter.h"
#include "Monster/SkinnyMonster.h"


ASkinnyMonster::ASkinnyMonster()
{
	MovementSpeed = 75.0f;
	Health = 100.0f;
	AttackDamage = 20.0f;
	AttackRange = 150.0f;
	MeleeStrikeCooldown = 2.5f;

	AttackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Range Sphere"));
	AttackRangeSphere->SetSphereRadius(AttackRange);
	AttackRangeSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackRangeSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackRangeSphere->SetupAttachment(GetCapsuleComponent());
	AttackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ASkinnyMonster::OnOverlapWithCharacter);
	AttackRangeSphere->OnComponentEndOverlap.AddDynamic(this, &ASkinnyMonster::OnEndOverlapWithCharacter);
}


void ASkinnyMonster::OnOverlapWithCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANBCharacter* OtherPawn = Cast<ANBCharacter>(OtherActor);
	if (OtherPawn)
	{
		StoredOtherActor = OtherActor;
		PerformAttack(OtherActor);
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "OnOverlappWithCharacter");
		
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::FromInt(MeleeStrikeCooldown * 10));
		/* Set re-trigger timer to re-check overlapping pawns at melee attack rate interval */
		GetWorldTimerManager().SetTimer(TimerHandle_MeleeAttack, this, &ASkinnyMonster::TriggerMeleeStrike, MeleeStrikeCooldown, true);

		USkinnyMonsterAnimInstance* AnimInstance = Cast<USkinnyMonsterAnimInstance>(GetMesh()->GetAnimInstance());

		if (AnimInstance)
		{
			// Set CanAttack in AnimInstance to true
			AnimInstance->CanAttack = true;
		}
	}
	
}

void ASkinnyMonster::OnEndOverlapWithCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		USkinnyMonsterAnimInstance* AnimInstance = Cast<USkinnyMonsterAnimInstance>(GetMesh()->GetAnimInstance());

		if (AnimInstance)
		{
			// Set CanAttack in AnimInstance to false
			AnimInstance->IsAttackFinished = true;
			AnimInstance->CanAttack = false;
		}
	}
}

void ASkinnyMonster::TriggerMeleeStrike()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "TriggerMeleeStrike");
	PerformAttack(StoredOtherActor);
}

void ASkinnyMonster::PerformAttack(AActor* HitActor)
{
	ANBCharacter* OtherPawn = Cast<ANBCharacter>(HitActor);


	if (LastStrikeTime > GetWorld()->GetTimeSeconds() - MeleeStrikeCooldown)
	{
		/* Set timer to start attacking as soon as the cooldown elapses. */
		if (!TimerHandle_MeleeAttack.IsValid())
		{
			// TODO: Set Timer
		}

		/* Attacked before cooldown expired */
		return;
	}
	if (OtherPawn)
	{
		LastStrikeTime = GetWorld()->GetTimeSeconds();
		
		ANBCharacter* mainChar = Cast<ANBCharacter>(GWorld->GetFirstPlayerController()->GetPawn());
		if (mainChar)
		{


			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Attack!");
			DecreaseCharacterHealth();
			//mainChar->DecreaseHealth(AttackDamage);
		}

	}
}