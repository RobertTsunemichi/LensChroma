// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
#include "BaseStructureClass.h"
#include "PlatformObject.generated.h"

UCLASS()
class LENSCHROMA_API APlatformObject : public ABaseStructureClass
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlatformObject();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCurrentlyVisible = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
