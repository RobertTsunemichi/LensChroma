// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseStructureClass.generated.h"

UCLASS()
class LENSCHROMA_API ABaseStructureClass : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseStructureClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	class UStaticMeshComponent* StaticMeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
