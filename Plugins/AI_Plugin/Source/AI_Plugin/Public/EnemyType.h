// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ModuleManager.h"
/**
 * 
 */
UENUM()
enum class EEnemyBehaviourType : uint8
{
	/* Patrol if set or stop. */
	Neutral,

	/* Slight higher speed and larger patro parameter. */
	Suspicious,
	/*Attack. . */
	Agression, 
	// Runs away. 
	Flee,
};

class AI_PLUGIN_API EnemyType
{
public:
	EnemyType();
	~EnemyType();
};
