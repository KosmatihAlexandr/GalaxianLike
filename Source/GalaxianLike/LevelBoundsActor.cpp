// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelBoundsActor.h"
#include "Components/BoxComponent.h"


ALevelBoundsActor::ALevelBoundsActor()
{
	PrimaryActorTick.bCanEverTick = false;

    USceneComponent* root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    SetRootComponent(root);

#if WITH_EDITOR

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("BoundsBox"));
    Box->SetupAttachment(RootComponent);
    Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);

#endif
}

FBox ALevelBoundsActor::GetBounds() const
{
    return FBox(GetActorLocation() + Border.Min, GetActorLocation() + Border.Max);
}

void ALevelBoundsActor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

#if WITH_EDITOR
    
    Box->SetBoxExtent(Border.GetExtent(), false);
    Box->SetRelativeLocation(Border.GetCenter());

#endif
}

