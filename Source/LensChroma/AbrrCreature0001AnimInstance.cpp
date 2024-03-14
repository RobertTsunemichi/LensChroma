// Fill out your copyright notice in the Description page of Project Settings.


#include "AbrrCreature0001AnimInstance.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"


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

void UAbrrCreature0001AnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (Pawn)
	{
		FVector Speed = Pawn->GetVelocity();
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);

		MovementSpeed = LateralSpeed.Size();
		//GEngine->AddOnScreenDebugMessage(40, 1.f, FColor::Black, FString::Printf(TEXT("BCC: MovingSpeed: %f"), MovementSpeed));

		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		if (AbrrCreatureCharacterClass == nullptr)
		{
			AbrrCreatureCharacterClass = Cast<AAbrrCreatureCharacterClass>(Pawn);
		}

		if (AbrrCreatureCharacterClass)
		{
			if (AbrrCreatureCharacterClass->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0)
				bIsAccelerating = true;
			else
				bIsAccelerating = false;

			bIsDead = AbrrCreatureCharacterClass->bIsDead;
			bIsInAttackRange = AbrrCreatureCharacterClass->bIsInAttackRange;
		}
	}
}