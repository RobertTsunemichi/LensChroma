// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbrrCreatureCharacterClass.generated.h"

UCLASS()
class LENSCHROMA_API AAbrrCreatureCharacterClass : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAbrrCreatureCharacterClass();

	float TestTimer = 0.f;
	float AttackTimer = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsNeutral = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasDied = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DrainPower = 5.f; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DrainRange = 225.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasPlayerTarget = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasBCharTarget = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* PlayerTargetActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* BCharTargetActor;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* FleeRangeCollider;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* AgroRangeCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInAttackRange = false;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* AttackRangeCollider;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentHealth;

	bool bHasFallen = false;
	float FallRes = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PlayerTargetLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BCharTargetLocation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void RandomMove();
	void FleeFromCharacter();
	void AgroCharacter(FVector NHeading);
	void AttackCharacter();
	void MoveToTarget(AActor* TargetActor);

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
