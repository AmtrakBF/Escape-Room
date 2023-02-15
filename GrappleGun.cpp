// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleGun.h"
#include "MainCharacter.h"
#include "Engine/World.h"
#include "CableComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"

AGrappleGun::AGrappleGun()
	: GrappleState(EGrappleState::GS_DETACHED), MaxGrappleDistance(2500.0f), GrapplingSpeed(1.0f), GrappleLocation(0.0f), GrappleDirection(0.0f), GrappleLength(0.0f), PlayerVelocity(0.0f), CharacterMovementComponent(nullptr)
{
	Grapple = CreateDefaultSubobject<UCableComponent>(TEXT("Grapple"));
	Grapple->SetupAttachment(RootComponent);
}

void AGrappleGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

// 	float distance = (Grapple->GetComponentLocation() - GrappleLocation).Size();
// 	if (distance < GrappleLength && GrappleState == EGrappleState::GS_PULLING)
// 	{
// 		GrappleLength = distance;
// 		Grapple->CableLength = distance;
// 	}

	if (CharacterMovementComponent && Player && GrappleState != EGrappleState::GS_DETACHED)
	{
		Grapple->EndLocation = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), GrappleLocation);
		GrappleDirection = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), GrappleLocation);

		//! So yeah I cheated but you learn new stuff when you cheat i guess

		FVector GrapplingDistance = GetActorLocation() - GrappleLocation;
		float DistanceMultiplier = GrapplingDistance.Dot(Player->GetVelocity());

		//! We turn distance into a direction
		UKismetMathLibrary::Vector_Normalize(GrapplingDistance);

		//! Multiply by -2 (constant for more force) to reverse direction (So we face towards the GrappleLocation)
		FVector Force = DistanceMultiplier * GrapplingDistance * -2;

		if (GrappleState == EGrappleState::GS_PULLING)
		{
			Force = GrappleDirection * CharacterMovementComponent->Mass * 1500 * (GrapplingSpeed * 1.1);
		}
		CharacterMovementComponent->AddForce(Force);
	}
}

void AGrappleGun::TogglePickupItem()
{
	AMainCharacter* TempPlayer = nullptr;
	if (Player)
	{
		TempPlayer = Player;
	}

	Super::TogglePickupItem();

	if (bIsPickedUp)
	{
		if (!Player)
			return;

		CharacterMovementComponent = Cast<UCharacterMovementComponent>(Player->GetMovementComponent());

		Player->OnLeftMouseButtonPressed.AddUniqueDynamic(this, &AGrappleGun::OnMouseLeftClickPressed);
		Player->OnRightMouseButtonPressed.AddUniqueDynamic(this, &AGrappleGun::OnMouseRightClickPressed);
		Player->OnRightMouseButtonReleased.AddUniqueDynamic(this, &AGrappleGun::OnMouseRightClickReleased);
	}
	else
	{
		if (!TempPlayer)
			return;

		DetachGrapple();

		if (CharacterMovementComponent)
		{
			CharacterMovementComponent->SetMovementMode(MOVE_Walking);
		}

		CharacterMovementComponent = nullptr;

		TempPlayer->OnLeftMouseButtonPressed.RemoveDynamic(this, &AGrappleGun::OnMouseLeftClickPressed);
		TempPlayer->OnRightMouseButtonPressed.RemoveDynamic(this, &AGrappleGun::OnMouseRightClickPressed);
		TempPlayer->OnRightMouseButtonReleased.RemoveDynamic(this, &AGrappleGun::OnMouseRightClickReleased);
	}
}

void AGrappleGun::OnMouseLeftClickPressed(AActor* Actor)
{
	if (GrappleState == EGrappleState::GS_ATTACHED || GrappleState == EGrappleState::GS_PULLING)
	{
		DetachGrapple();
	}
	else if (GrappleState == EGrappleState::GS_DETACHED)
	{
		AttachGrapple();
	}
}

void AGrappleGun::OnMouseRightClickPressed(AActor* Actor)
{
	if (GrappleState == EGrappleState::GS_ATTACHED)
	{
		GrappleState = EGrappleState::GS_PULLING;
	}
}

void AGrappleGun::OnMouseRightClickReleased(AActor* Actor)
{
	if (GrappleState == EGrappleState::GS_PULLING)
	{
		GrappleState = EGrappleState::GS_ATTACHED;
	}
}

void AGrappleGun::AttachGrapple()
{
	Grapple->SetVisibility(true);

	if (!Player)
		return;

	UWorld* World = Player->GetWorld();

	if (!World)
		return;

	FVector LineTraceEnd = (Player->CameraComponent->GetForwardVector() * MaxGrappleDistance) + Player->GetActorLocation();
	FHitResult Hit;

	bool bHitResult = World->LineTraceSingleByChannel(Hit, Player->GetActorLocation(), LineTraceEnd, ECollisionChannel::ECC_Visibility);

	if (bHitResult)
	{
		GrappleLocation = Hit.ImpactPoint;
		GrappleDirection = UKismetMathLibrary::GetDirectionUnitVector(Player->GetActorLocation(), Hit.ImpactPoint);

		GrappleState = EGrappleState::GS_ATTACHED;
		GrappleLength = (Grapple->GetComponentLocation() - GrappleLocation).Size();
	}
	else
	{
		GrappleState = EGrappleState::GS_DETACHED;
	}
}

void AGrappleGun::DetachGrapple()
{
	Grapple->SetVisibility(false);
	Grapple->EndLocation = FVector(0.0f);

	GrappleState = EGrappleState::GS_DETACHED;
}