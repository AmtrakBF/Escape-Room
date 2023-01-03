// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingActor.h"

// Sets default values
AFloatingActor::AFloatingActor()
	: bIsPickedUp(false), bIsCollectible(true), bIsRotatable(true), BobbingHeight(20.0f), RotatingSpeed(20.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	VisualMesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAssist(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));

	if (CubeVisualAssist.Succeeded())
	{
		VisualMesh->SetStaticMesh(CubeVisualAssist.Object);
		VisualMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	}
}

// Called when the game starts or when spawned
void AFloatingActor::BeginPlay()
{
	Super::BeginPlay();	
}

// Called every frame
void AFloatingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AmbientMovementOnTick(DeltaTime);
}

void AFloatingActor::AmbientMovementOnTick(float DeltaTime)
{
	if (bIsPickedUp)
		return;

	FVector NewLocation = GetActorLocation();
	FRotator NewRotation = GetActorRotation();

	float RunningTime = GetGameTimeSinceCreation();
	float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	NewLocation.Z += DeltaHeight * BobbingHeight;
	
	if (bIsRotatable)
	{
		float DeltaRotation = DeltaTime * RotatingSpeed;
		NewRotation.Yaw += DeltaRotation;
	}

	SetActorLocationAndRotation(NewLocation, NewRotation);
}
