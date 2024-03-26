// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveGamePlayer.generated.h"

/**
 * 
 */
UCLASS()
class LENSCHROMA_API USaveGamePlayer : public USaveGame
{
	GENERATED_BODY()

public:
	USaveGamePlayer();

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	FString PlayerName;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	uint32 UserSlot;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	bool bIsHoldingCamera;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	bool bIsCameraToggled;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	bool bIsLookingThroughCamera;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	bool bIsCurrentlyInGame;
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	bool bIsFromMenu;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	float PlayerSanity;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	float CameraBattery;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	bool bHasFlashlightOn;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	bool bHasRedFilter;
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	bool bHasGreenFilter;
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	bool bHasBlueFilter;

	// Score
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	float PlayerScore;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	bool bFinalStage;

	// Power filters
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	bool bHasPushFilter;
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	bool bHasPullFilter;
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	bool bHasEraseFilter;

	// Power filters active/inactive
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	bool bPushFilterActive;
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	bool bPullFilterActive;
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	bool bEraseFilterActive;

	// Additional variables to save when saving/loading from menu
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	FName CurrentMapName;
	
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	bool bHasFallen;
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	float FallRes;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	float PlayerLocationX;
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	float PlayerLocationY;
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	float PlayerLocationZ;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	float PlayerRotationRoll;
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	float PlayerRotationPitch;
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	float PlayerRotationYaw;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	float PlayerVelocityX;
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	float PlayerVelocityY;
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	float PlayerVelocityZ;

	UPROPERTY(VisibleAnywhere, Category = "Basic")
	float PlayerViewRoll;
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	float PlayerViewPitch;
	UPROPERTY(VisibleAnywhere, Category = "Basic")
	float PlayerViewYaw;

};
