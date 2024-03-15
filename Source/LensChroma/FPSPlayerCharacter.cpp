// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Actor.h"
#include "CameraInteractInterface.h"
#include "StrangeCamera.h"

#include "Components/SceneCaptureComponent2D.h"
#include <Kismet/GameplayStatics.h>

#include "StrangeCamera.h"
#include "CameraLens.h"
#include "Camera/PlayerCameraManager.h"

#include "PlatformObject.h"
#include "BaseCharacterClass.h"

#include "InteractableInterface.h"
#include "PortalActor.h"

#include "CameraFilterItem.h"

#include "BaseGameMode.h"
#include "SaveGamePlayer.h"

#include "Components/CapsuleComponent.h"
#include "BoostUpwardClass.h"

#include "Components/SpotLightComponent.h"

#include "AbrrCreatureCharacterClass.h"

#include "PlayerControllerClass.h"

#include "GameInstanceClass.h"
#include "NonCameraActor.h"
#include "SanityPillActor.h"
#include "CameraBatteryActor.h"

#include "FollowTargetPoints.h"
#include "GameFinishedActor.h"

// Sets default values
AFPSPlayerCharacter::AFPSPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(RootComponent);
	FirstPersonCamera->SetRelativeLocation(FVector(-24.f, 0.f, 64.f));
	FirstPersonCamera->bUsePawnControlRotation = true;

	StaticMeshComponentItem = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponentItem"));
	StaticMeshComponentItem->SetupAttachment(FirstPersonCamera);
	StaticMeshComponentItem->SetRelativeLocation(FVector(20, -10, -10));
	StaticMeshComponentItem->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));
	StaticMeshComponentItem->bCastDynamicShadow = false;

	Material = CreateDefaultSubobject<UMaterialInterface>(TEXT("Material"));

	/*CameraMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("CameraMaterial"));
	ConstructorHelpers::FObjectFinder<UMaterialInterface>CameraMaterialAsset(TEXT("Material'/Game/CameraCapture/StrangeCameraTarget_M.StrangeCameraTarget_M'"));
	CameraMaterial = CameraMaterialAsset.Object;*/

	CameraPushForce = 2000.f;

	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	Flashlight->SetupAttachment(FirstPersonCamera);
	Flashlight->Intensity = 5800.f;
	Flashlight->AttenuationRadius = 1500.f;
	Flashlight->InnerConeAngle = 30.f;
	Flashlight->OuterConeAngle = 45.f;
	Flashlight->SetHiddenInGame(true);

	UCapsuleComponent* CapsuleCollider = GetCapsuleComponent();
	if (CapsuleCollider)
	{
		CapsuleCollider->GetUnscaledCapsuleSize(DefaultColliderRadius, DefaultColliderHalfHeight);
	}

	PlayerSanity = 60.f;
	PlayerMaxSanity = 100.f;
	PlayerStamina = 100.f;
	PlayerMaxStamina = 100.f;
}

// Called when the game starts or when spawned
void AFPSPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	GameModeRef = Cast<ABaseGameMode>(GetWorld()->GetAuthGameMode());

	PlayerControllerClass = Cast<APlayerControllerClass>(GetController());

	// Set the below code in BeginPlay to change jump height if needed.
	// GetCharacterMovement()->JumpZVelocity = 600.f;
	 
	DefaultMaxMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;

	if (!bIsLookingThroughCamera) 
	{
		Flashlight->SetRelativeLocation(FVector(20.f, -10.f, -10.f));
	}

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT(">>> FPSChar BeginPlay <<<")));
	//// Check if went through portal
	//bool bLoadFromMenu = true;
	
	//// Only load from menu if you are not going through a portal
	//if (bLoadFromMenu)
	//{
	//	// Set the current map name based on the saved state
	//	USaveGamePlayer* LoadPlayerMenuInstance = Cast<USaveGamePlayer>(UGameplayStatics::CreateSaveGameObject(USaveGamePlayer::StaticClass()));
	//	LoadPlayerMenuInstance = Cast<USaveGamePlayer>(UGameplayStatics::LoadGameFromSlot(LoadPlayerMenuInstance->PlayerName, LoadPlayerMenuInstance->UserSlot));
	//	if (LoadPlayerMenuInstance) // If save found
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Loading into new world")));
	//		CurrentActiveLevel = LoadPlayerMenuInstance->CurrentMapName;
	//		LoadGameFromMenu();
	//	}
	//	else
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Loading into default world")));
	//		CurrentActiveLevel = CityLevelName;
	//	}
	//	FallRes = 0.f;
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Not loading from menu")));
	//}
}

// Called every frame
void AFPSPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//GEngine->AddOnScreenDebugMessage(555, 1.f, FColor::Green, FString::Printf(TEXT("Score:%f"), PlayerScore));


	// Player is dead don't process
	if (bIsDead) return;

	if (PlayerSanity > 0)
	{
		// Fall Damage
		if (GetVelocity().Z < 0)
		{
			// DEBUG
			//GEngine->AddOnScreenDebugMessage(112, 1.f, FColor::Red, FString::Printf(TEXT("vz:%f"), GetVelocity().Z));

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
				// DEBUG
				//GEngine->AddOnScreenDebugMessage(113, 15.f, FColor::Red, FString::Printf(TEXT("Fall Damage:%f"), FallRes));

				bHasFallen = false;
				DamagePlayer(FallRes);
				FallRes = 0.f;
			}
		}
	}
	else
	{
		bIsDead = true;
		//GEngine->AddOnScreenDebugMessage(1111, 10.f, FColor::Red, FString::Printf(TEXT("Player is Dead")));
	}

	TestDeltaTime += DeltaTime;
	if (TestDeltaTime >= 0.1f)
	{
		// DEBUG
		/*GEngine->AddOnScreenDebugMessage(200, 1.f, FColor::Red, FString::Printf(TEXT("RedFilter: %d"), bHasRedFilter));
		GEngine->AddOnScreenDebugMessage(201, 1.f, FColor::Red, FString::Printf(TEXT("GreenFilter: %d"), bHasGreenFilter));
		GEngine->AddOnScreenDebugMessage(202, 1.f, FColor::Red, FString::Printf(TEXT("BlueFilter: %d"), bHasBlueFilter));
		GEngine->AddOnScreenDebugMessage(203, 1.f, FColor::Red, FString::Printf(TEXT("PushFilter: %d"), bHasPushFilter));
		GEngine->AddOnScreenDebugMessage(204, 1.f, FColor::Red, FString::Printf(TEXT("PullFilter: %d"), bHasPullFilter));
		GEngine->AddOnScreenDebugMessage(205, 1.f, FColor::Red, FString::Printf(TEXT("EraseFilter: %d"), bHasEraseFilter));*/

		if (bIsSprinting)
		{
			//GEngine->AddOnScreenDebugMessage(305, 1.f, FColor::Red, FString::Printf(TEXT("Stamina: %f"), PlayerStamina));
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
			//GEngine->AddOnScreenDebugMessage(305, 1.f, FColor::Red, FString::Printf(TEXT("Stamina: %f"), PlayerStamina));
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
	PlayerInputComponent->BindAction("CycleLens", IE_Pressed, this, &AFPSPlayerCharacter::CycleCamera);
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

	PlayerInputComponent->BindAxis("CycleLens(Mouse)", this, &AFPSPlayerCharacter::CycleCamera);
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

void AFPSPlayerCharacter::CycleCamera(float Value)
{
	// Player is dead don't process
	if (bIsDead) return;

	int iValue = (int)Value % MAX_ITEMS;
	
	if (Value != 0)
	{
		// Change the currently selected item
		int DecCount = FMath::Abs(iValue);
		bool bIsIncrement = iValue > 0;
		while (DecCount > 0)
		{
			// Move selection
			if (bIsIncrement)
				SelectedItem++;
			else
				SelectedItem--;

			// Bounds check
			if (SelectedItem < 0)
				SelectedItem = MAX_ITEMS - 1;
			if (SelectedItem >= MAX_ITEMS)
				SelectedItem = 0;

			// Next step
			DecCount--;
		}

		// DEBUG
		//GEngine->AddOnScreenDebugMessage(10, 1.f, FColor::Purple, FString::Printf(TEXT("Selected Item is: %d"), SelectedItem));
	}
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

			/*FVector PushForce = CameraDirection * CameraPushForce;
			FVector PullForce = CamDir * CameraPushForce;*/

			// Action on trace result
			for (FHitResult SingleHitResult : BoxTraceResults)
			{
				AActor* HitActor = SingleHitResult.GetActor();

				ABaseCharacterClass* BCC = Cast<ABaseCharacterClass>(HitActor);
				if (BCC)
				{
					//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Hit Base Character")));
					//FVector CameraDirection = FirstPersonCamera->GetForwardVector() + FVector(0.f, 0.f, 0.5f);
					//BCC->LaunchCharacter(CameraDirection * CameraPushForce, true, true);
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
						//FVector CamDir = FVector(CameraDirection.X * (-1), CameraDirection.Y * (-1), CameraDirection.Z);
						BCC->LaunchCharacter(CamDir, true, true);
					}
				}
				else
				{
					AAbrrCreatureCharacterClass* ABC = Cast<AAbrrCreatureCharacterClass>(HitActor);
					if (ABC)
					{
						//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Hit Aberration Creature")));
						//FVector CameraDirection = FirstPersonCamera->GetForwardVector() + FVector(0.f, 0.f, 0.5f);
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
							//FVector CamDir = FVector(CameraDirection.X * (-1), CameraDirection.Y * (-1), CameraDirection.Z);
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
							//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Hit Other")));
								
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
			//CameraDirection *= InDirection;
			FVector CamDir = FVector(CameraDirection.X * InDirection, CameraDirection.Y * InDirection, CameraDirection.Z);
			StaticMesh1->AddImpulse(CamDir * CameraPushForce * StaticMesh1->GetMass());
		}
		else
		{
			// Don't care for non-mobile objects
		}
	}
}

void AFPSPlayerCharacter::MouseRMB()
{
	// Player is dead don't process
	if (bIsDead) return;

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Looking through camera")));

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

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, FString::Printf(TEXT("Activating shutter end!")));
	bLMBPushed = false;
}

void AFPSPlayerCharacter::MouseRMBEnd()
{
	// Player is dead don't process
	if (bIsDead) return;

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, FString::Printf(TEXT("Looking through camera end!")));

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
	//GEngine->AddOnScreenDebugMessage(351, 3.f, FColor::Blue, FString::Printf(TEXT("Sprinting 1: %f"), GetCharacterMovement()->MaxWalkSpeed));

	//GEngine->AddOnScreenDebugMessage(305, 1.f, FColor::Red, FString::Printf(TEXT("Stamina: %f"), PlayerStamina));
	///*PlayerStamina -= 3.f;f
	//if (PlayerStamina < 0.f)
	//{
	//	PlayerStamina = 0.f;
	//}*/

	//// Limit speed with no stamina
	//if (PlayerStamina <= 0.f)
	//{
	//	bIsSprinting = false;
	//}
	//
	//if (bIsSprinting)
	//{
	//	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxMoveSpeed * ThePlayerSpeed;
	//	GEngine->AddOnScreenDebugMessage(351, 3.f, FColor::Blue, FString::Printf(TEXT("Sprinting 1: %f"), GetCharacterMovement()->MaxWalkSpeed));
	//}
	//else
	//{
	//	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxMoveSpeed;
	//	GEngine->AddOnScreenDebugMessage(352, 3.f, FColor::Blue, FString::Printf(TEXT("Sprinting 2: %f"), GetCharacterMovement()->MaxWalkSpeed));
	//}

	//bIsSprinting = true;

	///*if (PlayerStamina > 0.f)
	//{
	//	bIsSprinting = true;
	//	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxMoveSpeed * 2;
	//	GEngine->AddOnScreenDebugMessage(310, 3.f, FColor::Blue, FString::Printf(TEXT("Sprinting 1: %f"), GetCharacterMovement()->MaxWalkSpeed));
	//}
	//else
	//{
	//	bIsSprinting = false;
	//	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxMoveSpeed;
	//	GEngine->AddOnScreenDebugMessage(311, 3.f, FColor::Blue, FString::Printf(TEXT("Sprinting 2: %f"), GetCharacterMovement()->MaxWalkSpeed));
	//}*/
	
} // Set the position to a magic number
void AFPSPlayerCharacter::MoveSprintEnd() 
{
	bIsSprinting = false;

	//GEngine->AddOnScreenDebugMessage(305, 1.f, FColor::Red, FString::Printf(TEXT("Stamina: %f"), PlayerStamina));
	/*PlayerStamina += PlayerStaminaRecoverySpeed;
	if (PlayerStamina > PlayerMaxStamina)
	{
		PlayerStamina = PlayerMaxStamina;
	}*/
	GetCharacterMovement()->MaxWalkSpeed = DefaultMaxMoveSpeed;

	/*GetCharacterMovement()->MaxWalkSpeed = DefaultMaxMoveSpeed; 
	GEngine->AddOnScreenDebugMessage(312, 3.f, FColor::Blue, FString::Printf(TEXT("Sprinting 3: %f"), GetCharacterMovement()->MaxWalkSpeed));*/
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
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("PushFilter: ")));
	// Player is dead don't process
	if (bIsDead) return;

	if (bIsLookingThroughCamera && bIsHoldingCamera && bHasPushFilter)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("PushFilter: Set")));
		bPullFilterActive = false;
		bPushFilterActive = true;
		bEraseFilterActive = false;
	}
}

void AFPSPlayerCharacter::TogglePullFilter()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("PullFilter: ")));
	// Player is dead don't process
	if (bIsDead) return;

	if (bIsLookingThroughCamera && bIsHoldingCamera && bHasPullFilter)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("PullFilter: Set")));
		bPullFilterActive = true;
		bPushFilterActive = false;
		bEraseFilterActive = false;
	}
}

void AFPSPlayerCharacter::ToggleEraseFilter()
{
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("EraseFilter: ")));
	// Player is dead don't process
	if (bIsDead) return;

	if (bIsLookingThroughCamera && bIsHoldingCamera && bHasEraseFilter)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("EraseFilter: Set")));
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
		//PlayerControllerClass->SetPause(true);
		PlayerControllerClass->TogglePauseMenu();
	}
	
	//// Player is dead don't process
	//if (bIsDead) return;

	//if ( Items[SelectedItem] != 0 )
	//{
	//	switch (Items[SelectedItem])
	//	{
	//	case 1:
	//		// TODO: implement the item management correctly
	//		GEngine->AddOnScreenDebugMessage(20, 5.f, FColor::Green, FString::Printf(TEXT("Drop Strange Camera")));
	//	default:
	//		break;
	//	}
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(20, 5.f, FColor::Blue, FString::Printf(TEXT("Nothing to drop!")));
	//}
}

void AFPSPlayerCharacter::ToggleCamera()
{
	// Player is dead don't process
	if (bIsDead) return;

	bIsCameraToggled = !bIsCameraToggled;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Black, FString::Printf(TEXT("Toggle look through camera, %d"), bIsCameraToggled));
}

void AFPSPlayerCharacter::CycleCamera()
{
	// Player is dead don't process
	if (bIsDead) return;

	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Cycling camera filter")));
}

void AFPSPlayerCharacter::SetWorldHiddenState(bool InBool)
{
	// DEBUG
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Showing hidden objects: %d"), InBool));

	TArray<AActor*> HiddenActors;

	// Toggle platform objects
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), PlatformObject, HiddenActors);

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
			/*SinglePObject->SetActorHiddenInGame(InBool);
			SinglePObject->SetActorEnableCollision(!InBool);*/
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
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("!!!Not final stage yet!!!")));
			}
			else
			{
				SinglePObject->SetActorHiddenInGame(false);
				//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("!!!Final stage!!!")));
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
			//StaticMeshComponentItemCamDefault->SetStaticMesh(ICameraInteractInterface::Execute_CameraGetMesh(StrangeCamera)->GetStaticMesh());

			// Get the strange camera material and apply it to the player attach position mesh
			Material = ICameraInteractInterface::Execute_CameraGetMaterial(StrangeCamera);
			//MaterialCamDefault = ICameraInteractInterface::Execute_CameraGetMaterial(StrangeCamera);

			// Set the static mesh material
			StaticMeshComponentItem->SetMaterial(0, Material);
			//StaticMeshComponentItemCamDefault->SetMaterial(0, MaterialCamDefault);

			// Set the selected item
			Items[SelectedItem] = 1;
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

	// TODO: Will need to check for filters applied to the camera
	// Specific portals should only show when specific filters are used.

	APortalActor* PActor = Cast<APortalActor>(InActor);
	if (PActor)
	{
		//// Update the currently active level
		//CurrentActiveLevel = PActor->LevelName;
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("CurrentActiveLevel: %s"), *CurrentActiveLevel.ToString()));

		if (PActor->ActorHasTag(FName("Dimension1_3")))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("!!!Going to Final Stage!!!")));
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

	ACameraFilterItem* CamFilterItem = Cast<ACameraFilterItem>(InActor);
	if (CamFilterItem)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Camera Filter Item: %d"), 117));
		// TODO: Add filter items that apply post processing effects as well as hide/unhide specific world objects
		// These filters will be used as the main part for the puzzles in the game.

		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Camera Filter Color: %s"), *CamFilterItem->FilterColor.ToString()));

		if (CamFilterItem->FilterColor == FName("Red"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Red filter")));
			PlayerScore += 100;
			bHasRedFilter = true;
		}
		if (CamFilterItem->FilterColor == FName("Green"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Green filter")));
			PlayerScore += 300;
			bHasGreenFilter = true;
		}
		if (CamFilterItem->FilterColor == FName("Blue"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Blue filter")));
			PlayerScore += 500;
			bHasBlueFilter = true;
		}
		if (CamFilterItem->FilterColor == FName("Push"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Push filter")));
			bHasPushFilter = true;
			bPushFilterActive = true;
			bPullFilterActive = false;
			bEraseFilterActive = false;
			PlayerScore += 500;
		}
		if (CamFilterItem->FilterColor == FName("Pull"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Pull filter")));
			bHasPullFilter = true;
			bPushFilterActive = false;
			bPullFilterActive = true;
			bEraseFilterActive = false;
			PlayerScore += 250;
		}
		if (CamFilterItem->FilterColor == FName("Erase"))
		{
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Erase filter")));
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
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Sanity pills!")));
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
		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Camera Battery!")));
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
			SelectedItem = LoadPlayerInstance->SelectedItem;
			for (int i = 0; i < MAX_ITEMS; i++)
			{
				Items[i] = LoadPlayerInstance->Items[i];
			}
			PlayerSanity = LoadPlayerInstance->PlayerSanity;
			PlayerMaxSanity = LoadPlayerInstance->PlayerMaxSanity;

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
			//bHasFlashlightOn = LoadPlayerInstance->bHasFlashlightOn;
			/*if (LoadPlayerInstance->bIsFromMenu)
			{

			}*/

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
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("Toggle Flashlight: %d"), Flashlight->bHiddenInGame));
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
	InCore->SelectedItem = SelectedItem;
	for (int i = 0; i < MAX_ITEMS; i++)
	{
		InCore->Items[i] = Items[i];
	}
	InCore->PlayerSanity = PlayerSanity;
	InCore->PlayerMaxSanity = PlayerMaxSanity;
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

	//InCore->CurrentMapName = CurrentActiveLevel;
}

//void AFPSPlayerCharacter::SaveGameFromMenu()
//{
//	USaveGamePlayer* SavePlayerInstance = Cast<USaveGamePlayer>(UGameplayStatics::CreateSaveGameObject(USaveGamePlayer::StaticClass()));
//
//	if (SavePlayerInstance)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Saving from menu")));
//		SaveCore(SavePlayerInstance);
//
//		SavePlayerInstance->bHasFallen = bHasFallen;
//		SavePlayerInstance->FallRes = FallRes;
//
//		// Player location
//		SavePlayerInstance->PlayerLocationX = GetActorLocation().X;
//		SavePlayerInstance->PlayerLocationY = GetActorLocation().Y;
//		SavePlayerInstance->PlayerLocationZ = GetActorLocation().Z;
//
//		//// Player rotation
//		//SavePlayerInstance->PlayerRotationRoll = GetActorRotation().Roll;
//		//SavePlayerInstance->PlayerRotationPitch = GetActorRotation().Pitch;
//		//SavePlayerInstance->PlayerRotationYaw = GetActorRotation().Yaw;
//
//		// Player velocity
//		SavePlayerInstance->PlayerVelocityX = GetVelocity().X;
//		SavePlayerInstance->PlayerVelocityY = GetVelocity().Y;
//		SavePlayerInstance->PlayerVelocityZ = GetVelocity().Z;
//
//		// Player view vector
//		//SavePlayerInstance->PlayerViewX = FirstPersonCamera->GetForwardVector().X;
//		SavePlayerInstance->PlayerViewRoll = FirstPersonCamera->GetComponentRotation().Roll;
//		SavePlayerInstance->PlayerViewPitch = FirstPersonCamera->GetComponentRotation().Pitch;
//		SavePlayerInstance->PlayerViewYaw = FirstPersonCamera->GetComponentRotation().Yaw;
//
//		//SavePlayerInstance->CurrentMapName = GetWorld()->GetMapName();
//		SavePlayerInstance->CurrentMapName = CurrentActiveLevel;
//
//
//		//FName LevelName = GetLevel()->GetFName();
//		//FString CurrentMapName = GetWorld()->GetMapName();
//		////GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("LevelName: %s"), *LevelName.ToString()));
//		//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("LevelName: %s"), *CurrentMapName));
//
//		//SavePlayerInstance->bIsFromMenu = true;
//
//		UGameplayStatics::SaveGameToSlot(SavePlayerInstance, SavePlayerInstance->PlayerName, SavePlayerInstance->UserSlot);
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Could not save from menu!")));
//	}
//}

//void AFPSPlayerCharacter::LoadGameFromMenu()
//{
//	USaveGamePlayer* LoadPlayerInstance = Cast<USaveGamePlayer>(UGameplayStatics::CreateSaveGameObject(USaveGamePlayer::StaticClass()));
//	LoadPlayerInstance = Cast<USaveGamePlayer>(UGameplayStatics::LoadGameFromSlot(LoadPlayerInstance->PlayerName, LoadPlayerInstance->UserSlot));
//
//	if (LoadPlayerInstance)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Loading from menu")));
//		if (PlayerControllerClass)
//		{
//			PlayerControllerClass->RemovePauseMenu();
//			PlayerControllerClass->SetPause(false);
//		}
//
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CurrentMap: %s"), *LoadPlayerInstance->CurrentMapName.ToString()));
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("CurretnLevel: %s"), *CurrentActiveLevel.ToString()));
//
//		if (LoadPlayerInstance->CurrentMapName != CurrentActiveLevel) // In the wrong world, load into the correct world
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Wrong world, reload")));
//
//			// Open the level
//			UGameplayStatics::OpenLevel(GetWorld(), LoadPlayerInstance->CurrentMapName);
//			return;
//		}
//		else
//		{
//			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Same world")));
//
//		}
//
//		bHasFallen = LoadPlayerInstance->bHasFallen;
//		FallRes = LoadPlayerInstance->FallRes;
//
//		// Load player state
//		bIsHoldingCamera = LoadPlayerInstance->bIsHoldingCamera;
//		bIsCameraToggled = LoadPlayerInstance->bIsCameraToggled;
//		bIsLookingThroughCamera = LoadPlayerInstance->bIsLookingThroughCamera;
//		SelectedItem = LoadPlayerInstance->SelectedItem;
//		for (int i = 0; i < MAX_ITEMS; i++)
//		{
//			Items[i] = LoadPlayerInstance->Items[i];
//		}
//		PlayerSanity = LoadPlayerInstance->PlayerSanity;
//		PlayerMaxSanity = LoadPlayerInstance->PlayerMaxSanity;
//
//		FVector PlayerLocation = FVector(LoadPlayerInstance->PlayerLocationX, LoadPlayerInstance->PlayerLocationY, LoadPlayerInstance->PlayerLocationZ);
//		SetActorLocation(PlayerLocation);
//		/*FRotator PlayerRotation = FRotator(LoadPlayerInstance->PlayerRotationPitch, LoadPlayerInstance->PlayerRotationYaw, LoadPlayerInstance->PlayerRotationRoll);
//		SetActorRotation(PlayerRotation);*/
//		// This isn't working
//		FRotator PlayerViewRotator = FRotator(LoadPlayerInstance->PlayerViewPitch, LoadPlayerInstance->PlayerViewYaw, LoadPlayerInstance->PlayerViewRoll);
//		FirstPersonCamera->SetWorldRotation(PlayerViewRotator);
//
//		FVector VelocityDirection = FVector(LoadPlayerInstance->PlayerVelocityX, LoadPlayerInstance->PlayerVelocityY, LoadPlayerInstance->PlayerVelocityZ);
//		LaunchCharacter(VelocityDirection, false, false);
//
//		if (bIsHoldingCamera)
//		{
//			// Get the strange camera mesh and material
//			AStrangeCamera* StrangeCamera = Cast<AStrangeCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), AStrangeCamera::StaticClass()));
//			// Get the strange camera mesh and apply it to the player attach position
//			StaticMeshComponentItem->SetStaticMesh(ICameraInteractInterface::Execute_CameraGetMesh(StrangeCamera)->GetStaticMesh());
//			// Get the strange camera material and apply it to the player attach position mesh
//			Material = ICameraInteractInterface::Execute_CameraGetMaterial(StrangeCamera);
//			StaticMeshComponentItem->SetMaterial(0, Material);
//
//			// Change world state based on is looking through camera
//			if (bIsLookingThroughCamera)
//			{
//				// Hide the camera in hand
//				StaticMeshComponentItem->SetVisibility(false);
//
//				// Show the hidden world objects
//				SetWorldHiddenState(false);
//			}
//
//			// Change flash light state
//			if (LoadPlayerInstance->bHasFlashlightOn && !bHasFlashlightOn)
//			{
//				ToggleFlashlight();
//			}
//		}
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Could not load from menu")));
//	}
//}

//void AFPSPlayerCharacter::ResetGame()
//{
//	UGameplayStatics::OpenLevel(GetWorld(), GetWorld()->GetMapName());
//}