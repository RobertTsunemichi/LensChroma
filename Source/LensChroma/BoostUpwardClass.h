// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Required header
#include "CoreMinimal.h"

// Inherit from base structure
#include "BaseStructureClass.h"

// Required generated
#include "BoostUpwardClass.generated.h"

UCLASS()
class LENSCHROMA_API ABoostUpwardClass : public ABaseStructureClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoostUpwardClass();

	// Collider for checking collision with the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* ColliderSphere;

	// Amount to boost the player by
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float BoostStrength;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Collider overlap event
	UFUNCTION()
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

};
