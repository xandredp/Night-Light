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
	MeleeStrikeCooldown = 1.0f;

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
	/* Stop any running attack timers */
	TimerHandle_MeleeAttack.Invalidate();

	PerformAttack(OtherActor);
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "OnOverlappWithCharacter");
	/* Set re-trigger timer to re-check overlapping pawns at melee attack rate interval */
	GetWorldTimerManager().SetTimer(TimerHandle_MeleeAttack, this, &ASkinnyMonster::TriggerMeleeStrike, MeleeStrikeCooldown, true);

	ANBCharacter* OtherPawn = Cast<ANBCharacter>(OtherActor);
	if (OtherPawn)
	{
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
	/* Apply damage to a single random pawn in range. */
	TArray<AActor*> Overlaps;
	AttackRangeSphere->GetOverlappingActors(Overlaps, ANBCharacter::StaticClass());
	
	for (int32 i = 0; i < Overlaps.Num(); i++)
	{
		ANBCharacter* OverlappingPawn = Cast<ANBCharacter>(Overlaps[i]);
		if (OverlappingPawn)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "TriggerMeleeStrike");
			PerformAttack(OverlappingPawn);
			//break; /* Uncomment to only attack one pawn maximum */
		}
	}

	/* No pawns in range, cancel the retrigger timer */
	if (Overlaps.Num() == 0)
	{
		TimerHandle_MeleeAttack.Invalidate();
	}
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