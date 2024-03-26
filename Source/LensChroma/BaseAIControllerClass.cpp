// Fill out your copyright notice in the Description page of Project Settings.

// Header
#include "BaseAIControllerClass.h"

// Constructor
ABaseAIControllerClass::ABaseAIControllerClass() { }

/*
 * Possession override 
 */
void ABaseAIControllerClass::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn == nullptr)
		return;
}