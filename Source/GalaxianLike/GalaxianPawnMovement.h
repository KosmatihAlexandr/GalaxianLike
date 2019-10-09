// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GalaxianPawnMovement.generated.h"

class ALevelBoundsActor;

/**
 * 
 */
UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class GALAXIANLIKE_API UGalaxianPawnMovement : public UPawnMovementComponent
{
	GENERATED_BODY()
	
    virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

    virtual float GetMaxSpeed() const override { return MaxSpeed; }

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloatingPawnMovement)
    float MaxSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloatingPawnMovement)
    float Acceleration;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FloatingPawnMovement)
    float Deceleration;

private:
    ALevelBoundsActor* Bounds;

public:
    UGalaxianPawnMovement(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void BeginPlay() override;

    virtual void ApplyControlInputToVelocity(float DeltaTime);
};
