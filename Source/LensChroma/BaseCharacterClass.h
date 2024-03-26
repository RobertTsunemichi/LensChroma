// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Required header
#include "CoreMinimal.h"

// 
#include "GameFramework/Character.h"

// Required generated 
#include "BaseCharacterClass.generated.h"

UCLASS()
class LENSCHROMA_API ABaseCharacterClass : public ACharacter
{
	GENERATED_BODY()

public:
	// Constructor
	ABaseCharacterClass();

	// Timer variables
	float ActionTimer = 0.f;

	// Fall damage variables
	bool bHasFallen = false;
	float FallRes = 0.f;

	// Character state variables
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasDied = false;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead = false;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFleeing = false;*/

	// Character stat variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSanity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentSanity;

protected:
	//// Called when the game starts or when spawned
	//virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void CalculateFallDamage();
	void CheckHealth();
	void RandomMove();

	/*UFUNCTION(BlueprintCallable)
	void FleeMove(FVector InVector);*/
};
