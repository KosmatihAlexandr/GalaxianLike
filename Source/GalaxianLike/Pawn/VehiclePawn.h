// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "VehiclePawn.generated.h"

class UGalaxianPawnMovement;
class UArmorComponent;
class ABaseProjectile;

UCLASS()
class GALAXIANLIKE_API AVehiclePawn : public APawn
{
	GENERATED_BODY()

protected:
    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Movement")
    UGalaxianPawnMovement* MovementComponent;

    UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Armor")
    UArmorComponent* ArmorComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Shooting")
    TSubclassOf<ABaseProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shooting")
    float ProjectileSpeed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shooting")
    uint8 ProjectileDamage;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Shooting")
    float FireRate;

private:
    FTimerHandle ShootingTimer;

    uint8 bIsShooting : 1;

public:
	AVehiclePawn();

    virtual void Tick(float DeltaTime) override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Shooting")
    void ShootPress();
    void ShootPress_Implementation();
    bool ShootPress_Validate();

    UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation, Category = "Shooting")
    void ShootRelease();
    void ShootRelease_Implementation();
    bool ShootRelease_Validate();

    UFUNCTION(BlueprintCallable, Category = "Shooting")
    bool CanShoot() const;

protected:
	virtual void BeginPlay() override;

    UFUNCTION(BlueprintImplementableEvent, Category = "Death")
    void OnDeath(AActor* DestroyedActor, AController* InstigatedBy, AActor* DamageCauser);

    UFUNCTION()
    void Death(AActor* DestroyedActor, AController* InstigatedBy, AActor* DamageCauser);

    UFUNCTION(BlueprintImplementableEvent, Category = "Shooting")
    void OnShoot();

    UFUNCTION(BlueprintNativeEvent, Category = "Shooting")
    FTransform GetProjectileSpawnTransform() const;
    FTransform GetProjectileSpawnTransform_Implementation() const;

private:
    void Shoot();

    UFUNCTION()
    void ShootTimerHandler();
};
