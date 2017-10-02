// Fill out your copyright notice in the Description page of Project Settings.

#include "NBSkinnyAI.h"



// Sets default values
ANBSkinnyAI::ANBSkinnyAI()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANBSkinnyAI::BeginPlay()
{
	Super::BeginPlay();
}

void ANBSkinnyAI::SimulateMeleeStrike()
{
	PlayAnimation(AttackAnimation);
	PlaySound(SoundIdle);
}
