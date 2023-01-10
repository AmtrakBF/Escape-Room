// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableItem.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "MainCharacter.h"

// Sets default values
ACollectableItem::ACollectableItem()
	: bCanBePlacedInInventory(true), bPhysicsEnabled(false), bIsPickedUp(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACollectableItem::BeginPlay()
{
	Super::BeginPlay();

	if (StaticMesh->IsAnySimulatingPhysics())
	{
		bPhysicsEnabled = true;
	}
}

void ACollectableItem::EnableCollisionToActor()
{
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACollectableItem::DisableCollisionToActor()
{
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

static float Time = 0.0f;
// Called every frame
void ACollectableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACollectableItem::TogglePickupItem()
{
	bIsPickedUp = !bIsPickedUp;

	if (bIsPickedUp)
	{
		Pickup();
	}
	else
	{
		Drop();
	}
}

void ACollectableItem::Pickup()
{
	if (!bCanBePlacedInInventory || !Player)
		return;

	if (bPhysicsEnabled)
	{
		StaticMesh->SetSimulatePhysics(false);
	}
	DisableCollisionToActor();

	Player->AddToInventory(this);
}

void ACollectableItem::Drop()
{
	if (!bCanBePlacedInInventory || !Player)
		return;

	EnableCollisionToActor();
	if (bPhysicsEnabled)
	{
		StaticMesh->SetSimulatePhysics(true);
	}

	Player->RemoveFromInventory(this);
}

