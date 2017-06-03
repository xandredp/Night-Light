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
	MeleeStrikeCooldown = 0.15f;
	AttackAttachPoint = TEXT("Attack_Socket");
	LastStrikeTime = 0.0f;

	AttackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Range Sphere"));
	AttackRangeSphere->SetSphereRadius(25);
	AttackRangeSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackRangeSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackRangeSphere->SetupAttachment(GetCapsuleComponent());
	AttackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ASkinnyMonster::OnOverlapWithCharacter);
	AttackRangeSphere->OnComponentEndOverlap.AddDynamic(this, &ASkinnyMonster::OnEndOverlapWithCharacter);
	AttackRangeSphere->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttackAttachPoint);


	AttackAnimTriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackAnim Range Sphere"));
	AttackAnimTriggerSphere->SetSphereRadius(AttackRange);
	AttackAnimTriggerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackAnimTriggerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackAnimTriggerSphere->SetupAttachment(GetCapsuleComponent());
	AttackAnimTriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ASkinnyMonster::OnOverlapStartAnim);
	AttackAnimTriggerSphere->OnComponentEndOverlap.AddDynamic(this, &ASkinnyMonster::OnEndOverlapStopAnim);
	
}


void ASkinnyMonster::OnOverlapWithCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ANBCharacter* OtherPawn = Cast<ANBCharacter>(OtherActor);
	if (OtherPawn)
	{
		StoredOtherActor = OtherActor;
		if (GetWorld()->TimeSeconds - LastStrikeTime > MeleeStrikeCooldown)
		{
			PerformAttack(OtherActor);
			LastStrikeTime = GetWorld()->GetTimeSeconds();
		}
	}
}

void ASkinnyMonster::OnEndOverlapWithCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{

	}
	
}

void ASkinnyMonster::OnOverlapStartAnim(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	USkinnyMonsterAnimInstance* AnimInstance = Cast<USkinnyMonsterAnimInstance>(GetMesh()->GetAnimInstance());
	ANBCharacter* OtherPawn = Cast<ANBCharacter>(OtherActor);
	if (OtherPawn)
	{
		if (AnimInstance)
		{
			// Set CanAttack in AnimInstance to true
			AnimInstance->CanAttack = true;
		}

	}

}

void ASkinnyMonster::OnEndOverlapStopAnim(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
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
	if (OtherPawn)
	{
		LastStrikeTime = GetWorld()->GetTimeSeconds();
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Attack!");
		DecreaseCharacterHealth();
	}
}