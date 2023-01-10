// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleItem.h"

// Sets default values
ADestructibleItem::ADestructibleItem()
	: HealthPoints(100.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void ADestructibleItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestructibleItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADestructibleItem::ReductHealthPoints(float Amount)
{
	HealthPoints -= Amount;

	if (HealthPoints <= 0)
	{
		DestroyBox();
	}
}

void ADestructibleItem::DestroyBox()
{
	//! Do some cool destruction animation or something
	
	Destroy();
}

