// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Required header
#include "CoreMinimal.h"

// Animation header
#include "Animation/AnimInstance.h"

// Required generated header
#include "Humanoid005AnimInstance.generated.h"

/*
 * The humanoid npc animation instance class
 */
UCLASS()
class LENSCHROMA_API UHumanoid005AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	// Update animation properties in blueprints
	UFUNCTION(BlueprintCallable, Category = "AnimationProperties")
	void UpdateAnimationProperties();

	// Character state variables
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float MovementSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsAccelerating;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsDead;

	// Character handles
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class APawn* Pawn;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	class ABaseCharacterClass* BaseCharacterClass;
};
