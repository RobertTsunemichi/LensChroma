// Fill out your copyright notice in the Description page of Project Settings.

// Headers
#include "AbrrCreature0001AnimInstance.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"

/* 
 * Initialize the handles to the owning pawn
 */
void UAbrrCreature0001AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			AbrrCreatureCharacterClass = Cast<AAbrrCreatureCharacterClass>(Pawn);
		}
	}
}

/* 
 * Update the animation properties called from within the animation event graph, specific to the
 * aberration creature animations.
 * The properties are used to set the various animations of the creature in the animation blueprints.
 */
void UAbrrCreature0001AnimInstance::UpdateAnimationProperties()
{
	// Get the pawn owner if not set yet.
	if (Pawn == nullptr)
		Pawn = TryGetPawnOwner();

	// If the pawn is set.
	if (Pawn)
	{
		// Get the movement speed.
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);
		MovementSpeed = LateralSpeed.Size();

		// Get the is flying state.
		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		// Get the creature handle if not set yet.
		if (AbrrCreatureCharacterClass == nullptr)
			AbrrCreatureCharacterClass = Cast<AAbrrCreatureCharacterClass>(Pawn);

		// If the creature handle is set.
		if (AbrrCreatureCharacterClass)
		{
			// Get the acceleration state.
			if (AbrrCreatureCharacterClass->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0)
				bIsAccelerating = true;
			else
				bIsAccelerating = false;

			// Get is-dead state.
			bIsDead = AbrrCreatureCharacterClass->bIsDead;
			// Get is-in-attack-range state.
			bIsInAttackRange = AbrrCreatureCharacterClass->bIsInAttackRange;
		}
	}
}