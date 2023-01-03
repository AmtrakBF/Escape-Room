// Fill out your copyright notice in the Description page of Project Settings.


#include "SlidingPlatform.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ASlidingPlatform::ASlidingPlatform()
	: MovementDelay(0.0f), MovementSpeed(1.0f), bOscillateMovement(false), MovementTimer(0.0f), bIsOpen(false), bIsOpening(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = PlatformMesh;
}

// Called when the game starts or when spawned
void ASlidingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	ClosedPosition = GetActorLocation();
	OpenedPosition = GetActorLocation() + PlatformTransform;
}

// Called every frame
void ASlidingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (MovementTimer > MovementDelay)
	{
		if (bIsOpen)
		{
			bIsClosing = true;
		} 
		else
		{
			bIsOpening = true;
		}
		MovementTimer = 0.0f;
	}

	if (bIsClosing)
	{
		SetActorLocation(FMath::Lerp(GetActorLocation(), ClosedPosition, MovementSpeed * DeltaTime));

		if (FVector::PointsAreNear(GetActorLocation(), ClosedPosition, 1.0))
		{
			bIsClosing = false;
			bIsOpen = false;
		}
	}
	
	if (bIsOpening)
	{
		SetActorLocation(FMath::Lerp(GetActorLocation(), OpenedPosition, MovementSpeed * DeltaTime));

		if (FVector::PointsAreNear(GetActorLocation(), OpenedPosition, 1.0))
		{
			bIsOpening = false;
			bIsOpen = true;
		}
	}

	MovementTimer += DeltaTime;
}