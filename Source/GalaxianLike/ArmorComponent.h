// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ArmorComponent.generated.h"

class AController;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDestroyDelegate, AActor*, DestroyedActor, AController*, InstigatedBy, AActor*, DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FArmorChangedDelegate, AActor*, Actor, uint8, DeltaArmor);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GALAXIANLIKE_API UArmorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FDestroyDelegate OnArmorDestroy;

    UPROPERTY(BlueprintAssignable)
    FArmorChangedDelegate OnArmorChaged;

protected:
    UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_Armor, Category = "Armor")
    uint8 Armor;

private:
    uint8 LocalArmor;

public:	
	UArmorComponent();

protected:
	virtual void BeginPlay() override;

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION()
    void ReciveDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

private:
    UFUNCTION()
    void OnRep_Armor();
};
