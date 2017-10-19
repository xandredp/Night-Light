// Fill out your copyright notice in the Description page of Project Settings.
#include "NBGameInstance.h"
#include "Core/No_Bark_Vs.h"
#include "Runtime/MoviePlayer/Public/MoviePlayer.h"

UNBGameInstance::UNBGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	LevelValue = 100;
}


void UNBGameInstance::Init()
{
	UGameInstance::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UNBGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMap.AddUObje.ct(this, &UNBGameInstance::EndLoadingScreen);
}


void UNBGameInstance::BeginLoadingScreen(const FString& MapName)
{
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
// 		LoadingScreen.bMoviesAreSkippable = true;
// 		LoadingScreen.MoviePaths.Add(TEXT("Loading_Screen"));
// 
// 		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
// 		GetMoviePlayer()->PlayMovie();

void UNBGameInstance::EndLoadingScreen()
{

}

