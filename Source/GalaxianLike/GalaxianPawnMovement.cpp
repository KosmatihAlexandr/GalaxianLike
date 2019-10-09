// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxianPawnMovement.h"
#include "LevelBoundsActor.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "EngineUtils.h"

UGalaxianPawnMovement::UGalaxianPawnMovement(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    MaxSpeed = 1200.f;
    Acceleration = 4000.f;
    Deceleration = 8000.f;

    ResetMoveState();
}

void UGalaxianPawnMovement::BeginPlay()
{
    Super::BeginPlay();

    for (TActorIterator<ALevelBoundsActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
    {
        if (!IsValid(*ActorItr))
        {
            continue;
        }

        Bounds = *ActorItr;
        break;
    }
}

void UGalaxianPawnMovement::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    if (ShouldSkipUpdate(DeltaTime))
    {
        return;
    }

    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (!PawnOwner || !UpdatedComponent)
    {
        return;
    }

    const AController* Controller = PawnOwner->GetController();
    if (Controller && Controller->IsLocalController())
    {
        if (Controller->IsLocalPlayerController() == true || Controller->IsFollowingAPath() == false || bUseAccelerationForPaths)
        {
            ApplyControlInputToVelocity(DeltaTime);
        }

        else if (IsExceedingMaxSpeed(MaxSpeed) == true)
        {
            Velocity = Velocity.GetUnsafeNormal() * MaxSpeed;
        }

        FVector Delta = Velocity * DeltaTime;

        if (!Delta.IsNearlyZero(1e-6f))
        {
            const FVector OldLocation = UpdatedComponent->GetComponentLocation();
            const FQuat Rotation = UpdatedComponent->GetComponentQuat();

            if (IsValid(Bounds))
            {
                const FBox boundBox = Bounds->GetBounds();
                if (!boundBox.IsInsideOrOn(OldLocation + Delta))
                {
                    Delta = boundBox.GetClosestPointTo(OldLocation + Delta) - OldLocation;
                }
            }

            FHitResult Hit(1.f);
            SafeMoveUpdatedComponent(Delta, Rotation, true, Hit);

            if (Hit.IsValidBlockingHit())
            {
                HandleImpact(Hit, DeltaTime, Delta);
                SlideAlongSurface(Delta, 1.f - Hit.Time, Hit.Normal, Hit, true);
            }

            FVector NewLocation = UpdatedComponent->GetComponentLocation();

            Velocity = ((NewLocation - OldLocation) / DeltaTime);
        }

        UpdateComponentVelocity();
    }
};

void UGalaxianPawnMovement::ApplyControlInputToVelocity(float DeltaTime)
{
    const FVector ControlAcceleration = GetPendingInputVector().GetClampedToMaxSize(1.f);

    const float AnalogInputModifier = (ControlAcceleration.SizeSquared() > 0.f ? ControlAcceleration.Size() : 0.f);
    const float MaxPawnSpeed = GetMaxSpeed() * AnalogInputModifier;
    const bool bExceedingMaxSpeed = IsExceedingMaxSpeed(MaxPawnSpeed);

    if (AnalogInputModifier > 0.f && !bExceedingMaxSpeed)
    {
        if (Velocity.SizeSquared() > 0.f)
        {
            Velocity = Velocity + (ControlAcceleration * Velocity.Size() - Velocity) * DeltaTime;
        }
    }
    else
    {
        if (Velocity.SizeSquared() > 0.f)
        {
            const FVector OldVelocity = Velocity;
            const float VelSize = FMath::Max(Velocity.Size() - FMath::Abs(Deceleration) * DeltaTime, 0.f);
            Velocity = Velocity.GetSafeNormal() * VelSize;

            if (bExceedingMaxSpeed && Velocity.SizeSquared() < FMath::Square(MaxPawnSpeed))
            {
                Velocity = OldVelocity.GetSafeNormal() * MaxPawnSpeed;
            }
        }
    }

    const float NewMaxSpeed = (IsExceedingMaxSpeed(MaxPawnSpeed)) ? Velocity.Size() : MaxPawnSpeed;
    Velocity += ControlAcceleration * FMath::Abs(Acceleration) * DeltaTime;
    Velocity = Velocity.GetClampedToMaxSize(NewMaxSpeed);

    ConsumeInputVector();
}
