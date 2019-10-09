// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelBoundsActor.generated.h"

UCLASS()
class GALAXIANLIKE_API ALevelBoundsActor : public AActor
{
	GENERATED_BODY()
	
protected:
    UPROPERTY(EditAnywhere, Category = "Border")
    FBox Border;

#if WITH_EDITORONLY_DATA

    class UBoxComponent* Box;

#endif

public:	
	ALevelBoundsActor();

    UFUNCTION(BlueprintCallable, Category = "Border")
    FBox GetBounds() const;
protected:
    virtual void OnConstruction(const FTransform& Transform) override;
};
