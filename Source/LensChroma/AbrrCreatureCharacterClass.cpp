// Fill out your copyright notice in the Description page of Project Settings.

// Required header
#include "AbrrCreatureCharacterClass.h"

// AI controller header
#include "BaseAIControllerClass.h"

// Collider sphere header
#include "Components/SphereComponent.h"

// Targets for creature headers
#include "FPSPlayerCharacter.h"
#include "BaseCharacterClass.h"

/*
 * Constructor
 */
AAbrrCreatureCharacterClass::AAbrrCreatureCharacterClass()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the agro collider
	AgroRangeCollider = CreateDefaultSubobject<USphereComponent>("AgroRangeCollider");
	AgroRangeCollider->SetupAttachment(RootComponent);

	// Set the creature default stats
	MaxHealth = 100.f;
	CurrentHealth = 100.f;
}

/*
 * Things to set once the map has loaded
 */
void AAbrrCreatureCharacterClass::BeginPlay()
{
	Super::BeginPlay();

	// Attach the begin/end overlap dynamic events to the collider
	AgroRangeCollider->OnComponentBeginOverlap.AddDynamic(this, &AAbrrCreatureCharacterClass::OnOverlapBegin);
	AgroRangeCollider->OnComponentEndOverlap.AddDynamic(this, &AAbrrCreatureCharacterClass::OnOverlapEnd);
}

/*
 * Things to process every frame
 */
void AAbrrCreatureCharacterClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Is dead, nothing to process
	if (!bIsDead)
	{
		// If the creature is faling
		CalculateFallDamage();

		// Is the creature dead?
		CheckIfDead();

		// Movement section
		CheckTargetAndMove();

		// Add time difference to timers
		ActionTimer += DeltaTime;
		AttackTimer += DeltaTime;
	}
}

void AAbrrCreatureCharacterClass::CalculateFallDamage()
{
	// If the creature is faling
	if (GetVelocity().Z < 0)
	{
		// Calculate the fall damage
		FallRes = GetVelocity().Z * (-1) - 1000;

		// If the fall damage potential result exceeds 0
		if (FallRes > 0)
		{
			// The creature has fallen enough to take damage
			bHasFallen = true;
			// Store the fall damage result to be applied next tick
			FallRes = FallRes / 10.f;
		}
	}
	else
	{
		// If the creature has falled enough to take damage
		if (bHasFallen)
		{
			// Reset the flag for has fallen enough
			bHasFallen = false;
			// Reduce the creatures current remaining health
			CurrentHealth -= FallRes;
			// reset the fall damage result
			FallRes = 0.f;
		}
	}
}

void AAbrrCreatureCharacterClass::CheckIfDead()
{
	if (CurrentHealth < 0)
	{
		// Set the creature is dead flag
		bIsDead = true; // Used for animation
		SetActorEnableCollision(false); // Remove the collision of dead actors
	}
}

void AAbrrCreatureCharacterClass::CheckTargetAndMove()
{
	// Move to player
	if (PlayerTargetActor) MoveToTarget(PlayerTargetActor, 0);
	// Move to npc
	else if (BCharTargetActor) MoveToTarget(BCharTargetActor, 1);
	// No target
	else
	{
		// Move to random location
		if (ActionTimer >= 4.f)
		{
			bIsInAttackRange = false;
			RandomMove();
			ActionTimer = 0.f;
		}
	}
}

/*
 * No targets are set so move about randomly
 */
void AAbrrCreatureCharacterClass::RandomMove()
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

/*
 * Move the aberration creature towards the target so it gets within attack range
 */
void AAbrrCreatureCharacterClass::MoveToTarget(AActor* TargetActor, int32 InInt)
{
	if (TargetActor)
	{
		// Check if should update the target agro
		if (ActionTimer >= CheckAgroTimer)
		{
			// Update the target agro
			if (TargetActor) AgroCharacter(TargetActor->GetActorLocation());
			// Reset the timer
			ActionTimer = 0.f;
		}

		// Calculate the distance difference to the target
		FVector DDist = TargetActor->GetActorLocation() - GetActorLocation();
		float DistanceDiff = sqrtf(DDist.X * DDist.X
			+ DDist.Y * DDist.Y
			+ DDist.Z * DDist.Z);

		// Check the distance difference
		if (DistanceDiff < DrainRange) { bIsInAttackRange = true; }
		else { bIsInAttackRange = false; }

		// If the distance to the target and the timer has timed out
		if (bIsInAttackRange && AttackTimer >= 0.5f)
		{
			// If a large number of targets will be used, use a switch statement
			if (InInt == 0) // Player == 0
			{
				// Damage the player
				AFPSPlayerCharacter* FPSChar = Cast<AFPSPlayerCharacter>(TargetActor);
				if (FPSChar)
				{
					FPSChar->PlayerSanity -= DrainPower;
				}
			}
			else // npc == 1
			{
				// Damage the other npc
				ABaseCharacterClass* BChar = Cast<ABaseCharacterClass>(TargetActor);
				if (BChar)
				{
					BChar->CurrentSanity -= DrainPower * 8; // drain npcs quicker
				}
			}

			// Reset the attack timer
			AttackTimer = 0.f;
		}
	}
}

/*
 * Collider sphere overlap event
 */
void AAbrrCreatureCharacterClass::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	// Is dead don't process
	if (!bIsDead)
	{
		// Check if the agro collider overlapped with the player
		AFPSPlayerCharacter* FPSChar = Cast<AFPSPlayerCharacter>(OtherActor);
		if (FPSChar) PlayerTargetActor = OtherActor;
		else
		{
			// Check if the agro collider overlapped with an npc
			ABaseCharacterClass* BaseChar = Cast<ABaseCharacterClass>(OtherActor);
			if (BaseChar) BCharTargetActor = OtherActor;
			// else do nothing
		}
	}
}

/*
 * Collider sphere overlap end event
 */
void AAbrrCreatureCharacterClass::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	// Is dead don't process
	if (!bIsDead)
	{
		// Unset player from target
		AFPSPlayerCharacter* FPSChar = Cast<AFPSPlayerCharacter>(OtherActor);
		if (FPSChar) PlayerTargetActor = nullptr;
		else
		{
			// Unset npc from target
			ABaseCharacterClass* BaseChar = Cast<ABaseCharacterClass>(OtherActor);
			if (BaseChar) BCharTargetActor = nullptr;
			// else do nothing
		}
	}
}

/*
 * Overlap event and timer occurred, move towards the targets location
 */
void AAbrrCreatureCharacterClass::AgroCharacter(FVector NHeading)
{
	ABaseAIControllerClass* AICon = Cast<ABaseAIControllerClass>(GetController());
	if (AICon) AICon->MoveToLocation(NHeading);
}
