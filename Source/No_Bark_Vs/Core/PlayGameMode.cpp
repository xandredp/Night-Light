// Fill out your copyright notice in the Description page of Project Settings.

#include "Core/No_Bark_Vs.h"
#include "Player/NBCharacter.h"
#include "Core/TypeClass.h"
#include "Core/PlayGameMode.h"


APlayGameMode::APlayGameMode()
{
	// set default pawn class to our Blueprinted character
	//static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/BluePrints/Character/BP_NBCharacter"));
	//if (PlayerPawnBPClass.Class != NULL)
	//{
	//	DefaultPawnClass = PlayerPawnBPClass.Class;
	//	
	//}
	GameModeState = EGameModeSoundType::General;
}
