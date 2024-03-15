// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SaveGamePlayer.h"

#include "FPSPlayerCharacter.generated.h"

UCLASS()
class LENSCHROMA_API AFPSPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSPlayerCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class ABaseGameMode* GameModeRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Controller")
	class APlayerControllerClass* PlayerControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* FirstPersonCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USpotLightComponent* Flashlight;

	// Game Level Names to load into
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CityLevelName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DimensionLevelName;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CurrentActiveLevel;*/

	// Blueprint classes to check for
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> CameraLensActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PlatformObject;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> NonCameraViewObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> FollowPointsObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> GameFinishedObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* AbbrCreatureMat1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UMaterialInterface* AbbrCreatureMat2;

	// Pickup items for healing etc.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> SanityPillObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> BatteryObject;

	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMeshComponentItem;
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* StaticMeshComponentItemCamDefault;*/

	// 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMaterialInterface* Material;
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMaterialInterface* MaterialCamDefault;*/
	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UMaterialInterface* CameraMaterial;*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsHoldingCamera = false;
	bool bIsCameraToggled = true;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bIsLookingThroughCamera = false;

	float DefaultMaxMoveSpeed;
	float Speed = 2.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bLMBPushed = false;

	const int MAX_ITEMS = 6;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int SelectedItem = 0;
	int Items[6] = { 0, 0, 0, 0, 0, 0 };

	float CameraPushForce;

	float DefaultColliderRadius;
	float DefaultColliderHalfHeight;

	// Player Sanity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	float PlayerSanity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	float PlayerMaxSanity;

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


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	float DamageTimeout = 1.f;

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

	void CycleCamera(float Value);

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
	void CycleCamera();
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
	//void ReCaptureCamera();

	UFUNCTION(BlueprintCallable)
	void SavePlayer(bool FromInGame);

	UFUNCTION(BlueprintCallable)
	bool LoadPlayer();

	UFUNCTION(BlueprintCallable)
	void SaveCore(USaveGamePlayer* InCore);

	/*UFUNCTION(BlueprintCallable)
	void ResetGame();*/

	/*UFUNCTION(BlueprintCallable)
	void SaveGameFromMenu();

	UFUNCTION(BlueprintCallable)
	void LoadGameFromMenu();*/
};
