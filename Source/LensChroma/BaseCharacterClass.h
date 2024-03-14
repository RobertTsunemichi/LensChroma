// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacterClass.generated.h"

UCLASS()
class LENSCHROMA_API ABaseCharacterClass : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacterClass();

	float TestTimer = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasDied = false;;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDead = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFleeing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSanity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentSanity;

	bool bHasFallen = false;
	float FallRes = 0.f;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void RandomMove();

	UFUNCTION(BlueprintCallable)
	void FleeMove(FVector InVector);
};
