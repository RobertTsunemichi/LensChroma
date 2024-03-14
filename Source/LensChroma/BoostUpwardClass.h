// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "BaseStructureClass.h"
#include "BoostUpwardClass.generated.h"

UCLASS()
class LENSCHROMA_API ABoostUpwardClass : public ABaseStructureClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoostUpwardClass();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* ColliderSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities")
	float BoostStrength;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

};
