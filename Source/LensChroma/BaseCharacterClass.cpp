// Fill out your copyright notice in the Description page of Project Settings.

// Header 
#include "BaseCharacterClass.h"

// Controller class header
#include "BaseAIControllerClass.h"

/*
 * Constructor
 */
ABaseCharacterClass::ABaseCharacterClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the character stats
	MaxSanity = 100.f;
	CurrentSanity = 100.f;
}

//// Called when the game starts or when spawned
//void ABaseCharacterClass::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}

/*
 * Calculate things every tick
 */
void ABaseCharacterClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Is dead, nothing to process
	if (bIsDead) return;

	// Calculate and apply fall damage
	CalculateFallDamage();
	// Check whether the character has died
	CheckHealth();

	/*if (!bIsFleeing)
	{
		
	}*/
	if (ActionTimer >= 4.f)
	{
		//GEngine->AddOnScreenDebugMessage(30, 3.f, FColor::Black, FString::Printf(TEXT("BCC: Moving!")));
		RandomMove();
		ActionTimer = 0.f;
	}

	ActionTimer += DeltaTime;
}

//// Called to bind functionality to input
//void ABaseCharacterClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//}

/*
 * Calculate the fall damage of the character
 */
void ABaseCharacterClass::CalculateFallDamage()
{
	// If the character is falling
	if (GetVelocity().Z < 0)
	{
		// Calculate the fall damage
		FallRes = GetVelocity().Z * (-1) - 1000;

		// If the fall damage is greater than 0
		if (FallRes > 0)
		{
			// Set the character has fallen flag
			bHasFallen = true;
			// Set the fall damage to apply on the next tick
			FallRes = FallRes / 10.f;
		}
	}
	else
	{
		// If the character has fallen
		if (bHasFallen)
		{
			// Reset the fallen tag
			bHasFallen = false;
			// Reduce the current stat of the character
			CurrentSanity -= FallRes;
			// Reset the fall damage
			FallRes = 0.f;
		}
	}
}

/*
 * Check if the character has died
 */
void ABaseCharacterClass::CheckHealth()
{
	if (CurrentSanity < 0)
	{
		// Used for animation
		//bHasDied = true;

		// Set the character is dead flag
		bIsDead = true;
		// Remove the characters collision
		SetActorEnableCollision(false);
	}
}

/*
 * Move npc to a random location within range
 */
void ABaseCharacterClass::RandomMove()
{
	ABaseAIControllerClass* AICon = Cast<ABaseAIControllerClass>(GetController());
	if (AICon)
	{
		FVector ActorLocation = GetActorLocation();
		FVector NHeading = FVector(
			ActorLocation.X + (int)FMath::FRandRange(500.f, 5000.f) - 2750.f,
			ActorLocation.Y + (int)FMath::FRandRange(500.f, 5000.f) - 2750.f,
			ActorLocation.Z);
		AICon->MoveToLocation(NHeading);
	}
}

//void ABaseCharacterClass::FleeMove(FVector InVector)
//{
//	ABaseAIControllerClass* AICon = Cast<ABaseAIControllerClass>(GetController());
//	if (AICon) AICon->MoveToLocation(InVector);
//}