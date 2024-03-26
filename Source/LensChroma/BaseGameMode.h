// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Required
#include "CoreMinimal.h"

// Header
#include "GameFramework/GameModeBase.h"

// Save game header
#include "SaveGamePlayer.h"

// Required generated
#include "BaseGameMode.generated.h"

/*
 * 
 */
UCLASS()
class LENSCHROMA_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Blueprint handle for the player start location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StartParameter")
	TSubclassOf<AActor> PlayerStartBP;

	// Blueprint for the player save game object
	TSubclassOf<USaveGame> PlayerSaveData;
	// Save game object
	USaveGamePlayer* SavePlayerObject;

	// Override function for choosing the player start location
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
};
