// Fill out your copyright notice in the Description page of Project Settings.

#include "Monsters/Monster Types/SkinnyMonster.h"
#include "Core/No_Bark_Vs.h"
#include "Player/NBCharacter.h"



ASkinnyMonster::ASkinnyMonster()
{
	MovementSpeed = 400.0f;
	Health = 100.0f;
	AttackDamage = 20.0f;
	AttackRange = 150.0f;
	MeleeStrikeCooldown = 5.0f;
	AttackAttachPoint = TEXT("Attack_Socket");
	LastStrikeTime = 0.0f;

	// This sphere component is attached to the hand to detect a hit
	AttackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Range Sphere"));
	AttackRangeSphere->SetSphereRadius(25);
	AttackRangeSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackRangeSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackRangeSphere->SetupAttachment(GetCapsuleComponent());
	AttackRangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ASkinnyMonster::OnOverlapWithCharacter);
	AttackRangeSphere->OnComponentEndOverlap.AddDynamic(this, &ASkinnyMonster::OnEndOverlapWithCharacter);
	
	// This sphere component surrounds the monster to determine attack range
	AttackAnimTriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackAnim Range Sphere"));
	AttackAnimTriggerSphere->SetSphereRadius(AttackRange);
	AttackAnimTriggerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackAnimTriggerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackAnimTriggerSphere->SetupAttachment(GetCapsuleComponent());
	AttackAnimTriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ASkinnyMonster::OnOverlapStartAnim);
	AttackAnimTriggerSphere->OnComponentEndOverlap.AddDynamic(this, &ASkinnyMonster::OnEndOverlapStopAnim);
	
	SkinnyMonsterSkeletal = GetMesh();
	IsAttacking = false;
}

void ASkinnyMonster::BeginPlay()
{
	Super::BeginPlay();
	AttackRangeSphere->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttackAttachPoint);
}


void ASkinnyMonster::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsAttacking)
	{
		if (StoredOtherActor)
		{			
			float ElapsedTimeSinceStrike = GetWorld()->TimeSeconds - LastStrikeTime;
			if (ElapsedTimeSinceStrike > MeleeStrikeCooldown)
			{
				SkinnyMonsterAnimInstance->Montage_Play(AttackAnimMontage, 1.0f);
				PerformAttack(StoredOtherActor);

				LastStrikeTime = GetWorld()->GetTimeSeconds();
			}
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "Cooldown :::      " + FString::SanitizeFloat(ElapsedTimeSinceStrike));
			}
		}
	}
}



void ASkinnyMonster::OnOverlapWithCharacter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetMonsterDead() == false)
	{
		ANBCharacter* OtherPawn = Cast<ANBCharacter>(OtherActor);
		if (OtherPawn)
		{
			StoredOtherActor = OtherPawn;
			if (GetWorld()->TimeSeconds - LastStrikeTime > MeleeStrikeCooldown)
			{
				if (IsAttacking)
				{
					PerformAttack(OtherActor);
					LastStrikeTime = GetWorld()->GetTimeSeconds();
				}
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
		ANBCharacter* OtherPawn = Cast<ANBCharacter>(OtherActor);
		if (OtherPawn)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, "Overlap!");
			//Check if it is not being stunned
			if (StunnedAnimPlaying != true)
			{
				//Check if anim exist
				if (AttackAnimMontage != NULL)
				{
					// check if it isn't attacking
					if (IsAttacking != true)
					{

						SkinnyMonsterAnimInstance = SkinnyMonsterSkeletal->GetAnimInstance();

						if (SkinnyMonsterAnimInstance != NULL)
						{
							IsAttacking = true;
							SkinnyMonsterAnimInstance->Montage_Play(AttackAnimMontage, 1.0f);
						}
					}
				}

			}
			
		}
	}
}

void ASkinnyMonster::OnEndOverlapStopAnim(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	//USkinnyMonsterAnimInstance* AnimInstance = Cast<USkinnyMonsterAnimInstance>(GetMesh()->GetAnimInstance());
	ANBCharacter* OtherPawn = Cast<ANBCharacter>(OtherActor);

	if (GetMonsterDead() == false)
	{
		if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
		{

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
