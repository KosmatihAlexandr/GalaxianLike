// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorComponent.h"
#include "GameFramework/Actor.h"
#include "UnrealNetwork.h"

UArmorComponent::UArmorComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
    bReplicates = true;
}

void UArmorComponent::BeginPlay()
{
	Super::BeginPlay();

    LocalArmor = Armor;

    GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UArmorComponent::ReciveDamage);
}

void UArmorComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UArmorComponent, Armor);
}

void UArmorComponent::ReciveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
    if (DamagedActor != GetOwner())
    {
        return;
    }

    Armor = FMath::Max<uint8>(Armor - Damage, 0);
    OnRep_Armor();

    if (Armor <= 0.f)
    {
        OnArmorDestroy.Broadcast(DamagedActor, InstigatedBy, DamageCauser);
    }
}

void UArmorComponent::OnRep_Armor()
{
    if (Armor == LocalArmor)
    {
        return;
    }

    OnArmorChaged.Broadcast(GetOwner(), LocalArmor - Armor);

    LocalArmor = Armor;
}

