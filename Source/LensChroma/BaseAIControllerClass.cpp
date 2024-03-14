// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAIControllerClass.h"

ABaseAIControllerClass::ABaseAIControllerClass()
{

}

void ABaseAIControllerClass::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (InPawn == nullptr)
	{
		return;
	}
}