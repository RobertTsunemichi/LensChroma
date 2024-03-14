// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CreatureSpawnCollider.generated.h"

UCLASS()
class LENSCHROMA_API ACreatureSpawnCollider : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACreatureSpawnCollider();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* ColliderSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> CreatureSpawnerBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> AberrantCreature;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bShouldDestroy = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SpawnerTag;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
