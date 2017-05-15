// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "AI_Plugin.h"


// This is NOT included by default in an empty project! It's required for replication and setting of the GetLifetimeReplicatedProps
#include "Net/UnrealNetwork.h"


#define WEAPON_TRACE				ECC_GameTraceChannel1



/** when you modify this, please note that this information can be saved with instances
* also DefaultEngine.ini [/Script/Engine.PhysicsSettings] should match with this list **/
#define SURFACE_DEFAULT				SurfaceType_Default
#define SURFACE_FLESH				SurfaceType1
#define SURFACE_ENEMYBODY			SurfaceType2
#define SURFACE_ENEMYHEAD			SurfaceType3
#define SURFACE_ENEMYLIMB			SurfaceType4

