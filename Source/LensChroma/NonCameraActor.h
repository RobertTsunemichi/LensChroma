// Fill out your copyright notice in the Description page of Project Settings.

// The actor that should only be visible when viewing without the camera

#pragma once

#include "CoreMinimal.h"
#include "BaseStructureClass.h"
#include "NonCameraActor.generated.h"

UCLASS()
class LENSCHROMA_API ANonCameraActor : public ABaseStructureClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANonCameraActor();

};
