// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreatureSpawnerBox.generated.h"

UCLASS()
class LENSCHROMA_API ACreatureSpawnerBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACreatureSpawnerBox();

	// Use this to have something to grab the object easier in the world
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* CreatureSpawnerSphere;

};
