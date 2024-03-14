// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SaveGameLevel.h"
#include "SaveGamePlayer.h"
#include "BaseGameMode.generated.h"

/**
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "StartParameter")
	TSubclassOf<AActor> PlayerStartBP;

	// Blueprint for the player save game object
	TSubclassOf<USaveGame> PlayerSaveData;

	USaveGamePlayer* SavePlayerObject;

	void InitPlayerSavedData();

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	
};
