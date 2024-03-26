// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// Required header
#include "CoreMinimal.h"
#include "GameFramework/Character.h"

// Save game header
#include "SaveGamePlayer.h"

// Required generated header
#include "FPSPlayerCharacter.generated.h"

UCLASS()
class LENSCHROMA_API AFPSPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSPlayerCharacter();

	// Game menu ui
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
	class APlayerControllerClass* PlayerControllerClass;

	// Player camera 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* FirstPersonCamera;

	// Flashlight
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpotLightComponent* Flashlight;

	/*
	 * TODO: Find a better way to implement this
	 */
	// Blueprint classes to check for
	// The platform used in showing/hiding
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PlatformObject;
	// Added this platform as part of the tutorials
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PlatformObject2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PortalActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> CameraFilterObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> BaseCharacterObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> AbrrCreatureObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> BoostUpwardPlatform;
	// The alternate platform type that only shows when not viewed through the camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> NonCameraViewObject;
	// Objects to get points
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> FollowPointsObject;
	// Object to end the game
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> GameFinishedObject;

	// Creature materials, used for showing transparent creature or not when spawned
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* AbbrCreatureMat1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* AbbrCreatureMat2;

	// Pickup for healing and charging the camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> SanityPillObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> BatteryObject;

	// Mesh component for showing the held camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMeshComponentItem;

	// Material for the camera in hand
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMaterialInterface* Material;

	// Toggles for held camera states
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsHoldingCamera = false;
	bool bIsCameraToggled = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsLookingThroughCamera = false;

	// Check the LMBpushed state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bLMBPushed = false;

	float CameraPushForce;

	// Player Sanity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	float PlayerSanity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	float PlayerMaxSanity;

	// Player movement variables
	float DefaultMaxMoveSpeed;
	// Crouched state
	float DefaultColliderRadius;
	float DefaultColliderHalfHeight;
	// Player Stamina
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PlayerStamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PlayerMaxStamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PlayerStaminaRecoverySpeed = 5.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsSprinting;
	float ThePlayerSpeed = 2.f;
	float ThePlayerDefaultSpeed = 2.f;

	// Camera battery
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CameraBattery = 100.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CameraMaxBattery = 1000.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	bool bIsDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerStats")
	bool bHasFlashlightOn = false;

	bool bHasFallen = false;
	float FallRes = 0.f;

	float TestDeltaTime = 0.f;

	// Player score
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PlayerScore = 0.f;

	// Filter keys
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHasRedFilter = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHasGreenFilter = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHasBlueFilter = false;

	// Power filters
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHasPushFilter = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHasPullFilter = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHasEraseFilter = false;

	// Power filters active/inactive
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bPushFilterActive = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bPullFilterActive = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bEraseFilterActive = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bHasFinished = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bFinalStage = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForwardBackward(float Value);
	void MoveLeftRight(float Value);

	void MouseLMB();
	void MouseRMB();
	void MouseLMBEnd();
	void MouseRMBEnd();

	void MoveJump();
	void MoveSprint();
	void MoveCrouch();
	void MoveSprintEnd();
	void MoveCrouchEnd();

	void ItemInteract();
	void PauseGame();
	void ToggleCamera();
	void ToggleFlashlight();

	void InteractWithPortal(AActor* InActor);
	void InteractWithCameraFilter(AActor* InActor);

	void SetWorldHiddenState(bool InBool);

	void ApplyImpulse(AActor* InActor, float InDirection);

	void DamagePlayer(float InDamage);

	void TogglePushFilter();
	void TogglePullFilter();
	void ToggleEraseFilter();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void PickUpCamera(AActor* InActor);

	UFUNCTION(BlueprintCallable)
	void SavePlayer(bool FromInGame);

	UFUNCTION(BlueprintCallable)
	bool LoadPlayer();

	UFUNCTION(BlueprintCallable)
	void SaveCore(USaveGamePlayer* InCore);
};
