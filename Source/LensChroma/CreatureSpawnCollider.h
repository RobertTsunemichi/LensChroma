// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Required
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

// Required generated
#include "CreatureSpawnCollider.generated.h"

UCLASS()
class LENSCHROMA_API ACreatureSpawnCollider : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACreatureSpawnCollider();

	// Use this to have something to grab the object easier in the world
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMesh;

	// The actual collider component
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* ColliderSphere;

	// The blueprint handles for the spawn target location and the spawn actor
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> CreatureSpawnerBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> AberrantCreature;

	// Collider control variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldDestroy = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SpawnerTag;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// On overlap function to handle the spawning
	UFUNCTION()
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
