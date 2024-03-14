// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIControllerClass.generated.h"

/**
 * 
 */
UCLASS()
class LENSCHROMA_API ABaseAIControllerClass : public AAIController
{
	GENERATED_BODY()
	
public:

	ABaseAIControllerClass();

	virtual void OnPossess(APawn* InPawn) override;
};
