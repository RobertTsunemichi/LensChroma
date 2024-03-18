// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Headers
#include "CoreMinimal.h"

#include "GameFramework/Character.h"

// Required generated header
#include "AbrrCreatureCharacterClass.generated.h"

/*
 * The base aberration creature class  
 */
UCLASS()
class LENSCHROMA_API AAbrrCreatureCharacterClass : public ACharacter
{
	GENERATED_BODY()

public:
	// Constructor
	AAbrrCreatureCharacterClass();

	// Timing variables
	float ActionTimer = 0.f;
	float AttackTimer = 0.f;
	float CheckAgroTimer = 0.5f;

	// Creature state
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead = false;
	bool bHasFallen = false;
	float FallRes = 0.f;

	// Creature combat variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DrainPower = 5.f; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DrainRange = 225.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* PlayerTargetActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* BCharTargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* AgroRangeCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInAttackRange = false;

	// Creature stats
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CalculateFallDamage();
	void CheckIfDead();
	void CheckTargetAndMove();
	void RandomMove();
	void AgroCharacter(FVector NHeading);
	void MoveToTarget(AActor* TargetActor, int32 InInt);

	UFUNCTION()
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

};
