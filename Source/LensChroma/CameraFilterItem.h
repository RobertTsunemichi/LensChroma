// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "BaseItemClass.h"
#include "CameraFilterItem.generated.h"

UCLASS()
class LENSCHROMA_API ACameraFilterItem : public ABaseItemClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraFilterItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName FilterColor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
