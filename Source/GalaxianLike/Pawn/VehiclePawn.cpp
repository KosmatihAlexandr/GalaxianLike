// Fill out your copyright notice in the Description page of Project Settings.


#include "VehiclePawn.h"
#include "BaseProjectile.h"
#include "GalaxianPawnMovement.h"
#include "ArmorComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

AVehiclePawn::AVehiclePawn()
{
	PrimaryActorTick.bCanEverTick = true;

    MovementComponent = CreateDefaultSubobject<UGalaxianPawnMovement>(TEXT("MovementComponent"));

    ArmorComponent = CreateDefaultSubobject<UArmorComponent>(TEXT("ArmorComponent"));

    ProjectileSpeed = 100.f;
    ProjectileDamage = 1;
    FireRate = 1;
}

void AVehiclePawn::BeginPlay()
{
	Super::BeginPlay();
	
    ArmorComponent->OnArmorDestroy.AddDynamic(this, &AVehiclePawn::Death);
}

FTransform AVehiclePawn::GetProjectileSpawnTransform_Implementation() const
{
    return GetActorTransform();
}

void AVehiclePawn::Shoot()
{
    if (!CanShoot())
    {
        return;
    }

    UWorld* world = GetWorld();

    if (!IsValid(world))
    {
        return;
    }

    FActorSpawnParameters spawnParametrs;
    spawnParametrs.Instigator = this;
    spawnParametrs.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    world->SpawnActor<ABaseProjectile>(ProjectileClass, GetProjectileSpawnTransform(), spawnParametrs);
    OnShoot();

    world->GetTimerManager().SetTimer(ShootingTimer, this, &AVehiclePawn::ShootTimerHandler, FireRate, false);
}

void AVehiclePawn::ShootTimerHandler()
{
    ShootingTimer.Invalidate();

    if (!bIsShooting)
    {
        return;
    }

    Shoot();
}

void AVehiclePawn::Death(AActor* DestroyedActor, AController* InstigatedBy, AActor* DamageCauser)
{
    OnDeath(DestroyedActor, InstigatedBy, DamageCauser);
}

void AVehiclePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVehiclePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AVehiclePawn::ShootPress_Implementation()
{
    bIsShooting = true;

    Shoot();
}

bool AVehiclePawn::ShootPress_Validate()
{
    return true;
}

void AVehiclePawn::ShootRelease_Implementation()
{
    bIsShooting = false;
}

bool AVehiclePawn::ShootRelease_Validate()
{
    return true;
}

bool AVehiclePawn::CanShoot() const
{
    return !ShootingTimer.IsValid();
}

