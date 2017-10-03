// Fill out your copyright notice in the Description page of Project Settings.

#include "NBSkinnyAI.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"



// Sets default values
ANBSkinnyAI::ANBSkinnyAI()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AttackAttachPoint = TEXT("Attack_Socket");

	// This sphere component is attached to the hand to detect a hit
	StrikePlayerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("StrikePlayerSphere"));
	StrikePlayerSphere->SetSphereRadius(25);
	StrikePlayerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	StrikePlayerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	StrikePlayerSphere->SetupAttachment(GetCapsuleComponent());
	StrikePlayerSphere->OnComponentBeginOverlap.AddDynamic(this, &ANBSkinnyAI::OnOverlapStrikeCharacter);
	StrikePlayerSphere->OnComponentEndOverlap.AddDynamic(this, &ANBSkinnyAI::OnEndOverlapStrikeCharacter);

	// This sphere component surrounds the monster to determine attack range
	AttackRangeAnimationTriggerSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackRangeAnimationTriggerSphere"));
	AttackRangeAnimationTriggerSphere->SetSphereRadius(150);
	AttackRangeAnimationTriggerSphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	AttackRangeAnimationTriggerSphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	AttackRangeAnimationTriggerSphere->SetupAttachment(GetCapsuleComponent());
	AttackRangeAnimationTriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ANBSkinnyAI::OnOverlapStartAnim);
	AttackRangeAnimationTriggerSphere->OnComponentEndOverlap.AddDynamic(this, &ANBSkinnyAI::OnEndOverlapStopAnim);

}

// Called when the game starts or when spawned
void ANBSkinnyAI::BeginPlay()
{
	Super::BeginPlay();
	StrikePlayerSphere->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttackAttachPoint);
}

void ANBSkinnyAI::SimulateMeleeStrike()
{
	PlayAnimation(AttackAnimation);
	PlaySound(SoundIdle);
}

void ANBSkinnyAI::OnOverlapStrikeCharacter(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{

	}
}

void ANBSkinnyAI::OnEndOverlapStrikeCharacter(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{

	}
}

void ANBSkinnyAI::OnOverlapStartAnim(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{

	}
	SimulateMeleeStrike();
}

void ANBSkinnyAI::OnEndOverlapStopAnim(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{

	}
}
