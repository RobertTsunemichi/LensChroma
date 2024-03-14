// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameInstanceClass.generated.h"

/**
 * 
 */
UCLASS()
class LENSCHROMA_API UGameInstanceClass : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TeleportationTag;
	
};
