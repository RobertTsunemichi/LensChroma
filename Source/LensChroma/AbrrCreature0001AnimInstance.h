// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AbrrCreatureCharacterClass.h"
#include "AbrrCreature0001AnimInstance.generated.h"

/*
 * The animation instance base class for the animation blueprint
 */
UCLASS()
class LENSCHROMA_API UAbrrCreature0001AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// Functions
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = "AnimationProperties")
	void UpdateAnimationProperties();

	// Handle Variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class APawn* Pawn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class AAbrrCreatureCharacterClass* AbrrCreatureCharacterClass;

	// State variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float MovementSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsAccelerating;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stat")
	bool bIsDead;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bIsInAttackRange;
	
};
