// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "GameHud.generated.h"

UENUM(BlueprintType)
enum class EHUDState : uint8
{
	Playing,
	Spectating,
	MatchEnd
};

UCLASS()
class NO_BARK_VS_API AGameHud : public AHUD
{
	GENERATED_BODY()

public:
	AGameHud();

	/* Current HUD state */
	EHUDState CurrentState;

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	UFUNCTION(BlueprintCallable, Category = "HUD")
		EHUDState GetCurrentState();

	///* An event hook to call HUD text events to display in the HUD. Blueprint HUD class must implement how to deal with this event. */
	//UFUNCTION(BlueprintImplementableEvent, Category = "HUDEvents")
	//	void MessageReceived(const FText& TextMessage);

	//* Event hook to update HUD state (eg. to determine visibility of widgets) */
	UFUNCTION(BlueprintNativeEvent, Category = "HUDEvents")
		void OnStateChanged(EHUDState NewState);

private:

	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;
	void DrawCrossHair();

};
