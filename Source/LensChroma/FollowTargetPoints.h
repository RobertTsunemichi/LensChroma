// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItemClass.h"
#include "FollowTargetPoints.generated.h"

UCLASS()
class LENSCHROMA_API AFollowTargetPoints : public ABaseItemClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFollowTargetPoints();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Points = 10.f;

	float TimeCounter = 0.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

};
