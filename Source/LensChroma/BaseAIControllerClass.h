// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Required header
#include "CoreMinimal.h"

#include "AIController.h"

// Required generated
#include "BaseAIControllerClass.generated.h"

/**
 * 
 */
UCLASS()
class LENSCHROMA_API ABaseAIControllerClass : public AAIController
{
	GENERATED_BODY()
	
public:
	// Constructor
	ABaseAIControllerClass();

	// Possession override
	virtual void OnPossess(APawn* InPawn) override;
};
