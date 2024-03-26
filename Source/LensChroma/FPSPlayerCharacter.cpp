// Fill out your copyright notice in the Description page of Project Settings.

// Required header
#include "FPSPlayerCharacter.h"

// Player camera header
#include "Camera/CameraComponent.h"

// Player mesh component
#include "Components/CapsuleComponent.h"

// Engine function header
#include <Kismet/GameplayStatics.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"

// Camera item interaction header
#include "CameraInteractInterface.h"
#include "StrangeCamera.h"

// Headers for casting
// Structures
#include "PlatformObject.h"
#include "NonCameraActor.h"
#include "PortalActor.h"
#include "BoostUpwardClass.h"
#include "GameFinishedActor.h"
// Items
#include "CameraFilterItem.h"
#include "SanityPillActor.h"
#include "CameraBatteryActor.h"
#include "FollowTargetPoints.h"
// Characters
#include "BaseCharacterClass.h"
#include "AbrrCreatureCharacterClass.h"
// Saving the player for transition of worlds
#include "BaseGameMode.h"
#include "SaveGamePlayer.h"

// Interfaces header
#include "InteractableInterface.h"

// Header for the flashlight
#include "Components/SpotLightComponent.h"

// Menu header
#include "PlayerControllerClass.h"

// Sets default values
AFPSPlayerCharacter::AFPSPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set up the player view camera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->SetRelativeLocation(FVector(-24.f, 0.f, 64.f));
	FirstPersonCamera->bUsePawnControlRotation = true;

	// Set the camera item location that the player should be able to see "in their hand"
	StaticMeshComponentItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponentItem"));
	StaticMeshComponentItem->SetupAttachment(FirstPersonCamera);
	StaticMeshComponentItem->SetRelativeLocation(FVector(20, -10, -10));
	StaticMeshComponentItem->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	StaticMeshComponentItem->bCastDynamicShadow = false;

	// Set up the material for the camera item "in their hand"
	Material = CreateDefaultSubobject<UMaterialInterface>(TEXT("Material"));

	// Set the camera item push power
	CameraPushForce = 2000.f;

	// Set up the player flashlight
	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	Flashlight->SetupAttachment(FirstPersonCamera);
	Flashlight->Intensity = 5800.f;
	Flashlight->AttenuationRadius = 1500.f;
	Flashlight->InnerConeAngle = 30.f;
	Flashlight->OuterConeAngle = 45.f;
	Flashlight->SetHiddenInGame(true);

	// Get the player collider to use for "crouch/uncrouch"
	UCapsuleComponent* CapsuleCollider = GetCapsuleComponent();
	if (CapsuleCollider)
	{
		CapsuleCollider->GetUnscaledCapsuleSize(DefaultColliderRadius, DefaultColliderHalfHeight);
	}

	// Set the player default stats
	PlayerSanity = 60.f;
	PlayerMaxSanity = 100.f;
	PlayerStamina = 100.f;
	PlayerMaxStamina = 100.f;
}

// Called when the game starts or when spawned
void AFPSPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get the player controller to be able to open the pause menu
	PlayerControllerClass = Cast<APlayerControllerClass>(GetController());

	// Set the below code in BeginPlay to change jump height if needed.
	// GetCharacterMovement()->JumpZVelocity = 600.f;
	 
	// Set the player default move speed
	DefaultMaxMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;

	// Set the default camera item location based on viewing-through-camera-toggle
	if (!bIsLookingThroughCamera) 
	{
		Flashlight->SetRelativeLocation(FVector(20.f, -10.f, -10.f));
	}
}

// Called every frame
void AFPSPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Player is dead don't process
	if (bIsDead) return;

	// Calculate player current/remaining sanity
	/*
	 * This calculation should be done as a delegate 
	 */
	if (PlayerSanity > 0)
	{
		// Fall Damage
		if (GetVelocity().Z < 0)
		{
			FallRes = GetVelocity().Z * (-1) - 1000;
			if (FallRes > 0)
			{
				FallRes = FallRes / 10.f;
				bHasFallen = true;
			}
		}
		else
		{
			if (bHasFallen)
			{
				bHasFallen = false;
				DamagePlayer(FallRes);
				FallRes = 0.f;
			}
		}
	}
	else
	{
		bIsDead = true;
	}

	TestDeltaTime += DeltaTime;
	if (TestDeltaTime >= 0.1f)
	{
		if (bIsSprinting)
		{
			PlayerStamina -= 3.f;
			if (PlayerStamina < 0.f)
			{
				PlayerStamina = 0.f;
				ThePlayerSpeed = 1.0f;
				GetCharacterMovement()->MaxWalkSpeed = DefaultMaxMoveSpeed * ThePlayerSpeed;
			}
		}
		else
		{
			PlayerStamina += PlayerStaminaRecoverySpeed;
			if (PlayerStamina > PlayerMaxStamina)
			{
				PlayerStamina = PlayerMaxStamina;
			}

			ThePlayerSpeed = ThePlayerDefaultSpeed;
		}

		CameraBattery += 0.1f;
		if (CameraBattery >= CameraMaxBattery)
		{
			CameraBattery = CameraMaxBattery;
		}

		TestDeltaTime = 0.f;
	}
}

// Called to bind functionality to input
void AFPSPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Movement key binds
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AFPSPlayerCharacter::MoveJump);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AFPSPlayerCharacter::MoveSprint);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AFPSPlayerCharacter::MoveCrouch);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AFPSPlayerCharacter::MoveSprintEnd);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AFPSPlayerCharacter::MoveCrouchEnd);

	PlayerInputComponent->BindAction("PushFilter", IE_Pressed, this, &AFPSPlayerCharacter::TogglePushFilter);
	PlayerInputComponent->BindAction("PullFilter", IE_Pressed, this, &AFPSPlayerCharacter::TogglePullFilter);
	PlayerInputComponent->BindAction("EraseFilter", IE_Pressed, this, &AFPSPlayerCharacter::ToggleEraseFilter);

	// Interacting key binds
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFPSPlayerCharacter::ItemInteract);
	PlayerInputComponent->BindAction("DropItem", IE_Pressed, this, &AFPSPlayerCharacter::PauseGame);
	PlayerInputComponent->BindAction("ToggleCameraView", IE_Pressed, this, &AFPSPlayerCharacter::ToggleCamera);
	PlayerInputComponent->BindAction("Flashlight", IE_Pressed, this, &AFPSPlayerCharacter::ToggleFlashlight);

	// Mouse key binds
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AFPSPlayerCharacter::MouseLMB);
	PlayerInputComponent->BindAction("PrimaryAction", IE_Released, this, &AFPSPlayerCharacter::MouseLMBEnd);
	PlayerInputComponent->BindAction("SecondaryAction", IE_Pressed, this, &AFPSPlayerCharacter::MouseRMB);
	PlayerInputComponent->BindAction("SecondaryAction", IE_Released, this, &AFPSPlayerCharacter::MouseRMBEnd);

	// Movement axis binds
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AFPSPlayerCharacter::MoveForwardBackward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AFPSPlayerCharacter::MoveLeftRight);

	// Observation axis binds
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
}

void AFPSPlayerCharacter::MoveForwardBackward(float Value)
{
	// Player is dead don't process
	if (bIsDead) return;

	FVector Forward = GetActorForwardVector();
	AddMovementInput(Forward, Value);
}

void AFPSPlayerCharacter::MoveLeftRight(float Value)
{
	// Player is dead don't process
	if (bIsDead) return;

	FVector Right = GetActorRightVector();
	AddMovementInput(Right, Value);
}

void AFPSPlayerCharacter::MouseLMB()
{
	// Player is dead don't process
	if (bIsDead) return;

	if (bIsLookingThroughCamera && bIsHoldingCamera)
	{
		// Set the boolean to show input presses
		bLMBPushed = true;

		if (bHasPushFilter || bHasPullFilter || bHasEraseFilter)
		{
			if (bPushFilterActive || bPullFilterActive)
			{
				if (CameraBattery < 1.f)
				{
					return;
				}
				CameraBattery -= 1.f;
			}
			if (bEraseFilterActive)
			{
				if (CameraBattery < 10.f)
				{
					return;
				}
				CameraBattery -= 10.f;
			}

			// Start and end points for the box multi hit
			FVector Start = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + 64.f);
			FVector End = Start + FirstPersonCamera->GetForwardVector() * 500.f;

			// Box multi hit parameters
			float BoxSize = 64.f;
			TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes;
			ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery3); // Pawn
			ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery4); // Physics Body
			TArray<AActor*> ActorsToIgnore;
			ActorsToIgnore.Add(this);
			TArray<FHitResult> BoxTraceResults;

			UKismetSystemLibrary::BoxTraceMultiForObjects(
				GetWorld(),
				Start,
				End,
				FVector(BoxSize, BoxSize, BoxSize),
				FRotator(0.f, 0.f, 0.f),
				ObjectTypes,
				true,
				ActorsToIgnore,
				EDrawDebugTrace::None,
				BoxTraceResults,
				true,
				FColor::Red,
				FColor::Blue,
				1.f);

			FVector CameraDirection = (FirstPersonCamera->GetForwardVector() + FVector(0.f, 0.f, 0.5f)) * CameraPushForce;
			FVector CamDir = FVector(CameraDirection.X * (-1), CameraDirection.Y * (-1), CameraDirection.Z);

			/*
			 *  TOOD: Find a better way to check what was overlapped with the box trace
			 */
			// Action on trace result
			for (FHitResult SingleHitResult : BoxTraceResults)
			{
				AActor* HitActor = SingleHitResult.GetActor();

				ABaseCharacterClass* BCC = Cast<ABaseCharacterClass>(HitActor);
				if (BCC)
				{
					BCC->LaunchCharacter(CameraDirection, true, true);

					if (bEraseFilterActive)
					{
						// Play some animation effects?
						BCC->Destroy();
					}
					else if (bPushFilterActive)
					{
						BCC->LaunchCharacter(CameraDirection, true, true);
					}
					else if (bPullFilterActive)
					{
						BCC->LaunchCharacter(CamDir, true, true);
					}
				}
				else
				{
					AAbrrCreatureCharacterClass* ABC = Cast<AAbrrCreatureCharacterClass>(HitActor);
					if (ABC)
					{
						ABC->LaunchCharacter(CameraDirection, true, true);

						if (bEraseFilterActive)
						{
							// Play some animation effects?
							ABC->Destroy();
						}
						else if (bPushFilterActive)
						{
							ABC->LaunchCharacter(CameraDirection, true, true);
						}
						else if (bPullFilterActive)
						{
							ABC->LaunchCharacter(CamDir, true, true);
						}
					}
					else
					{
						APlatformObject* PObj = Cast<APlatformObject>(HitActor);
						if (PObj)
						{
							if (bEraseFilterActive)
							{
								PObj->Destroy();
							}
						}
						else
						{								
							if (bPushFilterActive)
							{
								ApplyImpulse(HitActor, 1.f);
							}
							else if (bPullFilterActive)
							{
								ApplyImpulse(HitActor, -1.f);
							}
						}
					}
				}
			}
		}
	}
}

void AFPSPlayerCharacter::ApplyImpulse(AActor* InActor, float InDirection)
{
	// The below works on basic cube objects placed in the world
	UStaticMeshComponent* StaticMesh1 = Cast<UStaticMeshComponent>(InActor->GetRootComponent());
	if (StaticMesh1)
	{
		if (StaticMesh1->Mobility == EComponentMobility::Movable)
		{
			FVector CameraDirection = FirstPersonCamera->GetForwardVector();
			FVector CamDir = FVector(CameraDirection.X * InDirection, CameraDirection.Y * InDirection, CameraDirection.Z);
			StaticMesh1->AddImpulse(CamDir * CameraPushForce * StaticMesh1->GetMass());
		}
		// Don't care for non-mobile objects
	}
}

void AFPSPlayerCharacter::MouseRMB()
{
	// Player is dead don't process
	if (bIsDead) return;

	if (bIsCameraToggled)
	{
		bIsLookingThroughCamera = !bIsLookingThroughCamera;
	}
	else
	{
		bIsLookingThroughCamera = true;
	}

	if (bIsLookingThroughCamera && bIsHoldingCamera)
	{
		// Move the flash light position
		Flashlight->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

		// Hide the mesh instead
		StaticMeshComponentItem->SetVisibility(false);

		// Show hidden objects
		SetWorldHiddenState(false);
	}
}

void AFPSPlayerCharacter::MouseLMBEnd()
{
	// Player is dead don't process
	if (bIsDead) return;

	bLMBPushed = false;
}

void AFPSPlayerCharacter::MouseRMBEnd()
{
	// Player is dead don't process
	if (bIsDead) return;

	if (!bIsCameraToggled)
	{
		bIsLookingThroughCamera = false;
	}

	if (!bIsLookingThroughCamera && bIsHoldingCamera)
	{
		// Move the flash light position
		Flashlight->SetRelativeLocation(FVector(20.f, -10.f, -10.f));

		// Hide the mesh instead
		StaticMeshComponentItem->SetVisibility(true);

		// Hide hidden objects
		SetWorldHiddenState(true);
	}
}

void AFPSPlayerCharacter::MoveJump()
{
	// Player is dead don't process
	if (bIsDead) return;

	Jump(); 
}
void AFPSPlayerCharacter::MoveSprint() 
{
	bIsSprinting = true;
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxMoveSpeed * ThePlayerSpeed;
}

void AFPSPlayerCharacter::MoveSprintEnd() 
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxMoveSpeed;
}

void AFPSPlayerCharacter::MoveCrouch()
{
	// Player is dead don't process
	if (bIsDead) return;

	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxMoveSpeed * 0.25; // Set the position to a magic number

	UCapsuleComponent* CapsuleCollider = GetCapsuleComponent();
	if (CapsuleCollider)
	{
		CapsuleCollider->SetCapsuleSize(DefaultColliderRadius / 2.f, DefaultColliderHalfHeight / 2.f);
	}
}

void AFPSPlayerCharacter::MoveCrouchEnd() 
{
	// Player is dead don't process
	if (bIsDead) return;

	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxMoveSpeed; 

	UCapsuleComponent* CapsuleCollider = GetCapsuleComponent();
	if (CapsuleCollider)
	{
		CapsuleCollider->SetCapsuleSize(DefaultColliderRadius, DefaultColliderHalfHeight);
	}
}

void AFPSPlayerCharacter::TogglePushFilter()
{
	// Player is dead don't process
	if (bIsDead) return;

	if (bIsLookingThroughCamera && bIsHoldingCamera && bHasPushFilter)
	{
		bPullFilterActive = false;
		bPushFilterActive = true;
		bEraseFilterActive = false;
	}
}

void AFPSPlayerCharacter::TogglePullFilter()
{
	// Player is dead don't process
	if (bIsDead) return;

	if (bIsLookingThroughCamera && bIsHoldingCamera && bHasPullFilter)
	{
		bPullFilterActive = true;
		bPushFilterActive = false;
		bEraseFilterActive = false;
	}
}

void AFPSPlayerCharacter::ToggleEraseFilter()
{
	// Player is dead don't process
	if (bIsDead) return;

	if (bIsLookingThroughCamera && bIsHoldingCamera && bHasEraseFilter)
	{
		bPullFilterActive = false;
		bPushFilterActive = false;
		bEraseFilterActive = true;
	}
}

void AFPSPlayerCharacter::ItemInteract()
{
	// Player is dead don't process
	if (bIsDead) return;

	// Get all overlapped actors
	TArray<AActor*> OverlappedActorsArray;
	GetOverlappingActors(OverlappedActorsArray);

	for (AActor* Actor : OverlappedActorsArray)
	{
		// Check for overlapped camera
		PickUpCamera(Actor);

		// Check for overlapped portal
		InteractWithPortal(Actor);

		// Check for camera filter
		InteractWithCameraFilter(Actor);
	}
}

// Reuse this function as the menu key
void AFPSPlayerCharacter::PauseGame()
{
	if (PlayerControllerClass)
	{
		PlayerControllerClass->TogglePauseMenu();
	}
}

void AFPSPlayerCharacter::ToggleCamera()
{
	// Player is dead don't process
	if (bIsDead) return;

	bIsCameraToggled = !bIsCameraToggled;
}

void AFPSPlayerCharacter::SetWorldHiddenState(bool InBool)
{
	TArray<AActor*> HiddenActors;

	// Toggle platform objects
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlatformObject, HiddenActors);

	/*
	 * Should implement a hiding/unhiding interface for all these object below to reduce the number of checks/casting required.
	 */
	for (AActor* HiddenActor : HiddenActors)
	{
		APlatformObject* SinglePObject = Cast<APlatformObject>(HiddenActor);
		if (SinglePObject)
		{
			SinglePObject->SetActorHiddenInGame(InBool);
			SinglePObject->SetActorEnableCollision(!InBool);
		}
	}

	// Toggle platform objects
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlatformObject2, HiddenActors);

	for (AActor* HiddenActor : HiddenActors)
	{
		APlatformObject* SinglePObject = Cast<APlatformObject>(HiddenActor);
		if (SinglePObject)
		{
			SinglePObject->SetActorHiddenInGame(InBool);
			SinglePObject->SetActorEnableCollision(!InBool);
		}
	}

	// Toggle portal actors
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PortalActor, HiddenActors);

	for (AActor* HiddenActor : HiddenActors)
	{
		APortalActor* SinglePObject = Cast<APortalActor>(HiddenActor);
		if (SinglePObject)
		{
			SinglePObject->SetActorHiddenInGame(InBool);
			SinglePObject->SetActorEnableCollision(!InBool);
		}
	}

	// Toggle the boost platform actors
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BoostUpwardPlatform, HiddenActors);

	for (AActor* HiddenActor : HiddenActors)
	{
		ABoostUpwardClass* SinglePObject = Cast<ABoostUpwardClass>(HiddenActor);
		if (SinglePObject)
		{
			SinglePObject->SetActorHiddenInGame(InBool);
			SinglePObject->SetActorEnableCollision(!InBool);
		}
	}

	// Toggle the non camera view object actors
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), NonCameraViewObject, HiddenActors);

	for (AActor* HiddenActor : HiddenActors)
	{
		ANonCameraActor* SinglePObject = Cast<ANonCameraActor>(HiddenActor);
		if (SinglePObject)
		{
			SinglePObject->SetActorHiddenInGame(!InBool);
			SinglePObject->SetActorEnableCollision(InBool);
		}
	}

	// Sanity pill object actors
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), SanityPillObject, HiddenActors);

	for (AActor* HiddenActor : HiddenActors)
	{
		ASanityPillActor* SinglePObject = Cast<ASanityPillActor>(HiddenActor);
		if (SinglePObject)
		{
			SinglePObject->SetActorHiddenInGame(!InBool);
			SinglePObject->SetActorEnableCollision(InBool);
		}
	}
	
	// battery object actors
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), BatteryObject, HiddenActors);

	for (AActor* HiddenActor : HiddenActors)
	{
		ACameraBatteryActor* SinglePObject = Cast<ACameraBatteryActor>(HiddenActor);
		if (SinglePObject)
		{
			SinglePObject->SetActorHiddenInGame(!InBool);
			SinglePObject->SetActorEnableCollision(InBool);
		}
	}

	// Points object
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), FollowPointsObject, HiddenActors);

	for (AActor* HiddenActor : HiddenActors)
	{
		AFollowTargetPoints* SinglePObject = Cast<AFollowTargetPoints>(HiddenActor);
		if (SinglePObject)
		{
			SinglePObject->SetActorHiddenInGame(InBool);
			SinglePObject->SetActorEnableCollision(!InBool);
		}
	}

	// Aberration creature
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AbrrCreatureObject, HiddenActors);

	for (AActor* HiddenActor : HiddenActors)
	{
		AAbrrCreatureCharacterClass* SinglePObject = Cast<AAbrrCreatureCharacterClass>(HiddenActor);
		if (SinglePObject)
		{
			if (InBool)
			{
				SinglePObject->GetMesh()->SetMaterial(0, AbbrCreatureMat2);
			}
			else
			{
				SinglePObject->GetMesh()->SetMaterial(0, AbbrCreatureMat1);
			}
		}
	}

	// FinalStage
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), GameFinishedObject, HiddenActors);

	for (AActor* HiddenActor : HiddenActors)
	{
		AGameFinishedActor* SinglePObject = Cast<AGameFinishedActor>(HiddenActor);
		if (SinglePObject)
		{
			if (!bFinalStage)
			{
				SinglePObject->SetActorHiddenInGame(true);
			}
			else
			{
				SinglePObject->SetActorHiddenInGame(false);
			}
		}
	}
}

void AFPSPlayerCharacter::PickUpCamera(AActor* InActor)
{
	// Player is dead don't process
	if (bIsDead) return;

	if (!bIsHoldingCamera)
	{
		// Find the strange camera actor
		AStrangeCamera* StrangeCamera = Cast<AStrangeCamera>(InActor);
		if (StrangeCamera)
		{
			// Get the strange camera mesh and apply it to the player attach position
			StaticMeshComponentItem->SetStaticMesh(ICameraInteractInterface::Execute_CameraGetMesh(StrangeCamera)->GetStaticMesh());

			// Get the strange camera material and apply it to the player attach position mesh
			Material = ICameraInteractInterface::Execute_CameraGetMaterial(StrangeCamera);

			// Set the static mesh material
			StaticMeshComponentItem->SetMaterial(0, Material);

			// Set the selected item
			bIsHoldingCamera = true;

			// Remove the strange camera actor from the world
			StrangeCamera->Destroy();
		}
	}
}

void AFPSPlayerCharacter::InteractWithPortal(AActor* InActor)
{
	// Player is dead don't process
	if (bIsDead) return;

	APortalActor* PActor = Cast<APortalActor>(InActor);
	if (PActor)
	{
		/*
		 * The FName tag of "Dimension1_3" was arbitrarily set on the portal within the UE5 GUI
		 */
		if (PActor->ActorHasTag(FName("Dimension1_3")))
		{
			bFinalStage = true;
		}

		// Save the player state first
		SavePlayer(true);

		// Change levels
		IInteractableInterface::Execute_ActivateInteraction(PActor, this, 174);
	}
}

void AFPSPlayerCharacter::InteractWithCameraFilter(AActor* InActor)
{
	// Player is dead don't process
	if (bIsDead) return;

	/*
	 * Should implement an interactable interface for all the interactable objects below.
	 * Then just call the interface for has-interface actors
	 */
	ACameraFilterItem* CamFilterItem = Cast<ACameraFilterItem>(InActor);
	if (CamFilterItem)
	{
		/*
		 * Use a switch-case here
		 */
		if (CamFilterItem->FilterColor == FName("Red"))
		{
			PlayerScore += 100;
			bHasRedFilter = true;
		}
		if (CamFilterItem->FilterColor == FName("Green"))
		{
			PlayerScore += 300;
			bHasGreenFilter = true;
		}
		if (CamFilterItem->FilterColor == FName("Blue"))
		{
			PlayerScore += 500;
			bHasBlueFilter = true;
		}
		if (CamFilterItem->FilterColor == FName("Push"))
		{
			bHasPushFilter = true;
			bPushFilterActive = true;
			bPullFilterActive = false;
			bEraseFilterActive = false;
			PlayerScore += 500;
		}
		if (CamFilterItem->FilterColor == FName("Pull"))
		{
			bHasPullFilter = true;
			bPushFilterActive = false;
			bPullFilterActive = true;
			bEraseFilterActive = false;
			PlayerScore += 250;
		}
		if (CamFilterItem->FilterColor == FName("Erase"))
		{
			bHasEraseFilter = true;
			bPushFilterActive = false;
			bPullFilterActive = false;
			bEraseFilterActive = true;
			PlayerScore += 1000;
		}
		CamFilterItem->Destroy();
	}

	// Sanity pills
	ASanityPillActor* SanityPillItem = Cast<ASanityPillActor>(InActor);
	if (SanityPillItem)
	{
		PlayerSanity += SanityPillItem->RecoveryAmount;
		if (PlayerSanity > PlayerMaxSanity)
		{
			PlayerSanity = PlayerMaxSanity;
		}

		SanityPillItem->Destroy();
	}

	// Camera Battery
	ACameraBatteryActor* CameraBatteryItem = Cast<ACameraBatteryActor>(InActor);
	if (CameraBatteryItem)
	{
		CameraBattery += CameraBatteryItem->ChargeAmount;
		if (CameraBattery >= CameraMaxBattery)
		{
			CameraBattery = CameraMaxBattery;
		}

		CameraBatteryItem->Destroy();
	}
}

void AFPSPlayerCharacter::SavePlayer(bool FromInGame)
{
	USaveGamePlayer* SavePlayerInstance = Cast<USaveGamePlayer>(UGameplayStatics::CreateSaveGameObject(USaveGamePlayer::StaticClass()));

	if (SavePlayerInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Created the save player instance. Adding player state to instance."));
		// Variables to always save
		SaveCore(SavePlayerInstance);

		// Went through portal bool
		SavePlayerInstance->bIsCurrentlyInGame = FromInGame;

		UE_LOG(LogTemp, Warning, TEXT("Created the save player instance. Saving the player instance."));
		// Save
		UGameplayStatics::SaveGameToSlot(SavePlayerInstance, SavePlayerInstance->PlayerName + FString("State"), SavePlayerInstance->UserSlot);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create the save player instance."));
	}
}
 
bool AFPSPlayerCharacter::LoadPlayer()
{
	USaveGamePlayer* LoadPlayerInstance = Cast<USaveGamePlayer>(UGameplayStatics::CreateSaveGameObject(USaveGamePlayer::StaticClass()));

	LoadPlayerInstance = Cast<USaveGamePlayer>(UGameplayStatics::LoadGameFromSlot(LoadPlayerInstance->PlayerName + FString("State"), LoadPlayerInstance->UserSlot));

	if (LoadPlayerInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("Created the player load instance."));
		if (LoadPlayerInstance->bIsCurrentlyInGame)
		{
			UE_LOG(LogTemp, Warning, TEXT("Loading from within the game."));
			// Load player state
			bIsHoldingCamera = LoadPlayerInstance->bIsHoldingCamera;
			bIsCameraToggled = LoadPlayerInstance->bIsCameraToggled;
			bIsLookingThroughCamera = LoadPlayerInstance->bIsLookingThroughCamera;
			PlayerSanity = LoadPlayerInstance->PlayerSanity;
			CameraBattery = LoadPlayerInstance->CameraBattery;

			bHasRedFilter = LoadPlayerInstance->bHasRedFilter;
			bHasGreenFilter = LoadPlayerInstance->bHasGreenFilter;
			bHasBlueFilter = LoadPlayerInstance->bHasBlueFilter;

			bHasPushFilter = LoadPlayerInstance->bHasPushFilter;
			bHasPullFilter = LoadPlayerInstance->bHasPullFilter;
			bHasEraseFilter = LoadPlayerInstance->bHasEraseFilter;

			bPushFilterActive = LoadPlayerInstance->bPushFilterActive;
			bPullFilterActive = LoadPlayerInstance->bPullFilterActive;
			bEraseFilterActive = LoadPlayerInstance->bEraseFilterActive;

			PlayerScore = LoadPlayerInstance->PlayerScore;

			// Change world state based on is looking through camera
			if (bIsLookingThroughCamera)
			{
				// Hide the camera in hand
				StaticMeshComponentItem->SetVisibility(false);

				// Show the hidden world objects
				SetWorldHiddenState(false);
			}

			// Change flash light state
			if (LoadPlayerInstance->bHasFlashlightOn)
			{
				ToggleFlashlight();
			}

			UE_LOG(LogTemp, Warning, TEXT("Overriding the player state without the in-game state."));
			// Remove the bIsCurrentlyInGame flag
			SavePlayer(false);

			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("BAD_LOAD. Was loaded from outside game state. This should only occur when the game is first started or restarted."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to create the load player instance."));
	}
	return false;
}

void AFPSPlayerCharacter::ToggleFlashlight()
{
	// Player is dead don't process
	if (bIsDead) return;
	if (!bIsHoldingCamera) return;

	bHasFlashlightOn = !bHasFlashlightOn;
	Flashlight->SetHiddenInGame(!Flashlight->bHiddenInGame);
}

void AFPSPlayerCharacter::DamagePlayer(float InDamage)
{
	PlayerSanity -= InDamage;
}

void AFPSPlayerCharacter::SaveCore(USaveGamePlayer* InCore)
{
	InCore->bIsHoldingCamera = bIsHoldingCamera;
	InCore->bIsCameraToggled = bIsCameraToggled;
	InCore->bIsLookingThroughCamera = bIsLookingThroughCamera;
	InCore->PlayerSanity = PlayerSanity;
	InCore->CameraBattery = CameraBattery;
	InCore->bHasFlashlightOn = bHasFlashlightOn;

	InCore->bHasRedFilter = bHasRedFilter;
	InCore->bHasGreenFilter = bHasGreenFilter;
	InCore->bHasBlueFilter = bHasBlueFilter;

	InCore->bHasPushFilter = bHasPushFilter;
	InCore->bHasPullFilter = bHasPullFilter;
	InCore->bHasEraseFilter = bHasEraseFilter;

	InCore->bPushFilterActive = bPushFilterActive;
	InCore->bPullFilterActive = bPullFilterActive;
	InCore->bEraseFilterActive = bEraseFilterActive;

	InCore->PlayerScore = PlayerScore;

	InCore->bFinalStage = bFinalStage;
}
