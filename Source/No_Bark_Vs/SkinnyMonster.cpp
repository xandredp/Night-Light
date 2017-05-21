// Fill out your copyright notice in the Description page of Project Settings.

#include "No_Bark_Vs.h"
#include "SkinnyMonster.h"


ASkinnyMonster::ASkinnyMonster()
{
	MovementSpeed = 75.0f;
	Health = 100.0f;
	AttackDamage = 20.0f;
	AttackRange = 100.0f;

	AttackRangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("Attack Range Sphere"));
	AttackRangeSphere->AttachTo(RootComponent);
	AttackRangeSphere->SetSphereRadius(AttackRange);
}