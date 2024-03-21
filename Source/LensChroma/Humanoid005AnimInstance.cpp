// Fill out your copyright notice in the Description page of Project Settings.

// Header
#include "Humanoid005AnimInstance.h"
#include "BaseCharacterClass.h"

// Character handles headers
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"

/*
 * Initialization for the animation instance
 */
void UHumanoid005AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// Set the character handle if not set
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn) BaseCharacterClass = Cast<ABaseCharacterClass>(Pawn);
	}
}

/*
 * Update the animation properties which are used in the animation of the humanoid npc character
 */
void UHumanoid005AnimInstance::UpdateAnimationProperties()
{
	// Set the character handle if not set
	if (Pawn == nullptr) Pawn = TryGetPawnOwner();

	// If the pawn has been set
	if (Pawn)
	{
		// Get the movement state
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		// Set the handle if the character handle is not set
		if (BaseCharacterClass == nullptr)
		{
			BaseCharacterClass = Cast<ABaseCharacterClass>(Pawn);
		}

		// If the character handle is set
		if (BaseCharacterClass)
		{
			// Get the character stat
			if (BaseCharacterClass->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0)
				bIsAccelerating = true;
			else
				bIsAccelerating = false;

			// Set the character is dead state for animation
			bIsDead = BaseCharacterClass->bIsDead;
		}
	}
}