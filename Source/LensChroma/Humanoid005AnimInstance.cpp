// Fill out your copyright notice in the Description page of Project Settings.


#include "Humanoid005AnimInstance.h"
//#include "BaseCharacterClass.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/CharacterMovementComponent.h"

void UHumanoid005AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
		if (Pawn)
		{
			BaseCharacterClass = Cast<ABaseCharacterClass>(Pawn);
		}
	}
}

void UHumanoid005AnimInstance::UpdateAnimationProperties()
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

		if (BaseCharacterClass == nullptr)
		{
			BaseCharacterClass = Cast<ABaseCharacterClass>(Pawn);
		}

		if (BaseCharacterClass)
		{
			if (BaseCharacterClass->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0)
				bIsAccelerating = true;
			else
				bIsAccelerating = false;

			bIsFleeing = BaseCharacterClass->bIsFleeing;
			bIsDead = BaseCharacterClass->bIsDead;
		}
	}
}