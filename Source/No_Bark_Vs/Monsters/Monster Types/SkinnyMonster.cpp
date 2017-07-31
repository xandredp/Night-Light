// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/No_Bark_Vs.h"
#include "Player/NBCharacter.h"
#include "Monsters/Monster Types/SkinnyMonster.h"


ASkinnyMonster::ASkinnyMonster()
{
	MovementSpeed = 400.0f;
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
	


	AttackAnimTriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackAnim Range Sphere"));
	AttackAnimTriggerSphere->SetSphereRadius(AttackRange);
	AttackAnimTriggerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackAnimTriggerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackAnimTriggerSphere->SetupAttachment(GetCapsuleComponent());
	AttackAnimTriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ASkinnyMonster::OnOverlapStartAnim);
	AttackAnimTriggerSphere->OnComponentEndOverlap.AddDynamic(this, &ASkinnyMonster::OnEndOverlapStopAnim);
	
	//AnimInstance = Cast<USkinnyMonsterAnimInstance>(GetMesh()->GetAnimInstance());


}

void ASkinnyMonster::BeginPlay()
{
	Super::BeginPlay();
	AttackRangeSphere->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttackAttachPoint);
}

void ASkinnyMonster::OnOverlapWithCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetMonsterDead() == false)
	{
		USkinnyMonsterAnimInstance* AnimInstance = Cast<USkinnyMonsterAnimInstance>(GetMesh()->GetAnimInstance());
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
}

void ASkinnyMonster::OnEndOverlapWithCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{

	}
}

void ASkinnyMonster::OnOverlapStartAnim(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (GetMonsterDead() == false)
	{
		USkinnyMonsterAnimInstance* AnimInstance = Cast<USkinnyMonsterAnimInstance>(GetMesh()->GetAnimInstance());
		ANBCharacter* OtherPawn = Cast<ANBCharacter>(OtherActor);
		if (OtherPawn)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Attack range sphere!");
			if (AnimInstance)
			{
				// Set CanAttack in AnimInstance to true
				AnimInstance->CanAttack = true;
			}
		}
	}
}

void ASkinnyMonster::OnEndOverlapStopAnim(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	USkinnyMonsterAnimInstance* AnimInstance = Cast<USkinnyMonsterAnimInstance>(GetMesh()->GetAnimInstance());
	ANBCharacter* OtherPawn = Cast<ANBCharacter>(OtherActor);

	if (GetMonsterDead() == false)
	{
		if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
		{
			if (AnimInstance)
			{
				// Set CanAttack in AnimInstance to false
				AnimInstance->IsAttackFinished = true;
				AnimInstance->CanAttack = false;
			}
		}
	}
}

void ASkinnyMonster::PerformAttack(AActor* HitActor)
{
	if (GetMonsterDead() == false)
	{
		ANBCharacter* OtherPawn = Cast<ANBCharacter>(HitActor);
		if (OtherPawn)
		{
			LastStrikeTime = GetWorld()->GetTimeSeconds();
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Attack!");
			
			if (OtherPawn->CurrentHealth > 0)
			{
				PlayAttackSound();
			}
			else
			{
				PlayDeathAttackSound();
			}
			DecreaseCharacterHealth();
		}
	}
}