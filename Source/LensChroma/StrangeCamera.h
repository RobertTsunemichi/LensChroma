// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItemClass.h"
#include "CameraInteractInterface.h"
#include "StrangeCamera.generated.h"

UCLASS()
class LENSCHROMA_API AStrangeCamera : public ABaseItemClass, public ICameraInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStrangeCamera();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMaterialInterface* Material;

	virtual UStaticMeshComponent* CameraGetMesh_Implementation() override;
	virtual UMaterialInterface* CameraGetMaterial_Implementation() override;

};
