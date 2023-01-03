// Fill out your copyright notice in the Description page of Project Settings.


#include "ZipLine.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "MainCharacter.h"
#include "Components/SceneComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AZipLine::AZipLine()
	: ZipliningSpeed(1.0f), Player(nullptr), bPlayerIsOnZipline(false), ConstantZiplineSpeed(1000.0f), ZiplineDirection(0.0f), NormalizedZiplineStartToEndDistance(0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	Collision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetupAttachment(RootComponent);

	ZiplineStartPoint = CreateDefaultSubobject<USceneComponent>(TEXT("StartPoint"));
	ZiplineStartPoint->SetupAttachment(RootComponent);

	ZiplineEndPoint = CreateDefaultSubobject<USceneComponent>(TEXT("EndPoint"));
	ZiplineEndPoint->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AZipLine::BeginPlay()
{
	Super::BeginPlay();

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AZipLine::BeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AZipLine::EndOverlap);

	NormalizedZiplineStartToEndDistance = FMath::PointDistToSegment(ZiplineStartPoint->GetComponentLocation(), ZiplineEndPoint->GetComponentLocation(), ZiplineEndPoint->GetComponentLocation());
	ZiplineDirection = UKismetMathLibrary::GetDirectionUnitVector(ZiplineStartPoint->GetComponentLocation(), ZiplineEndPoint->GetComponentLocation());

}

// Called every frame
void AZipLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bPlayerIsOnZipline && Player)
	{
		UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(Player->GetMovementComponent());

		if (CharacterMovementComponent)
		{
			CharacterMovementComponent->Velocity = ZiplineDirection * (ZipliningSpeed * ConstantZiplineSpeed);
		}

		if ((ZiplineEndPoint->GetComponentLocation() - GetActorLocation()).Size() < 1.0f)
		{
			bPlayerIsOnZipline = false;
			SetPlayerMovementFlying(false);
		}
	}

}

void AZipLine::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<AMainCharacter>(OtherActor);
	if (Player->IsValidLowLevel())
	{
		Player->OnInteractionKeyPressed.AddUniqueDynamic(this, &AZipLine::OnInteractionButtonPressed);
	}
}

void AZipLine::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Player)
	{
		Player->OnInteractionKeyPressed.RemoveDynamic(this, &AZipLine::OnInteractionButtonPressed);
		if (bPlayerIsOnZipline)
		{
			SetPlayerMovementFlying(false);
			bPlayerIsOnZipline = false;
		}
	}
	Player = nullptr;
}

void AZipLine::OnInteractionButtonPressed(AActor* player)
{
	if (!Player)
		return;

	if (!bPlayerIsOnZipline)
	{
		AttachPlayerToZipline();
	}
	else
	{
		SetPlayerMovementFlying(false);
		bPlayerIsOnZipline = false;
	}
}

void AZipLine::AttachPlayerToZipline()
{
	if (!Player)
		return;

	SetPlayerMovementFlying(true);

	FVector AttachmentLocation = FMath::ClosestPointOnLine(ZiplineStartPoint->GetComponentLocation(), ZiplineEndPoint->GetComponentLocation(), Player->GetActorLocation());
	Player->SetActorLocation(AttachmentLocation);
	bPlayerIsOnZipline = true;
}

void AZipLine::SetPlayerMovementFlying(bool bIsFlying)
{
	if (!Player)
		return;

	if (bIsFlying)
	{
		UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(Player->GetMovementComponent());

		if (CharacterMovementComponent)
		{
			Player->MovementMultiplier = FVector(0.0f, 0.0f, 0.0f);
			CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
		}
	}
	else
	{
		UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(Player->GetMovementComponent());

		if (CharacterMovementComponent)
		{
			Player->MovementMultiplier = FVector(1.0f, 1.0f, 1.0f);
			CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Walking);
		}
	}
}

