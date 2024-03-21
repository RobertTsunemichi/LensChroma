// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CameraInteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCameraInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * The interface class that should be used
 */
class LENSCHROMA_API ICameraInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UStaticMeshComponent* CameraGetMesh();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	UMaterialInterface* CameraGetMaterial();

};
