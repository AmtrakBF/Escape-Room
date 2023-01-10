// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappleGun.h"
#include "MainCharacter.h"
#include "Engine/World.h"
#include "CableComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"

AGrappleGun::AGrappleGun()
	: GrappleState(EGrappleState::GS_DETACHED), GrappleLocation(0.0f), GrappleDirection(0.0f), CharacterMovementComponent(nullptr), MaxGrappleDistance(2500.0f), GrapplingSpeed(1.0f)
{
	Grapple = CreateDefaultSubobject<UCableComponent>(TEXT("Grapple"));
	Grapple->SetupAttachment(RootComponent);
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
	}
	else
	{
		GrappleState = EGrappleState::GS_DETACHED;

		if (CharacterMovementComponent)
		{
			CharacterMovementComponent->SetMovementMode(MOVE_Walking);
		}
	}
}

void AGrappleGun::DetachGrapple()
{
	Grapple->SetVisibility(false);
	Grapple->EndLocation = FVector(0.0f);

	GrappleState = EGrappleState::GS_DETACHED;

	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->SetMovementMode(MOVE_Walking);
	}
}

void AGrappleGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GrappleState == EGrappleState::GS_ATTACHED)
	{
		Grapple->EndLocation = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), GrappleLocation);
	}

	if (GrappleState == EGrappleState::GS_PULLING && Player)
	{
		Grapple->EndLocation = UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), GrappleLocation);
		GrappleDirection = UKismetMathLibrary::GetDirectionUnitVector(Player->GetActorLocation(), GrappleLocation);

		if (CharacterMovementComponent)
		{
			CharacterMovementComponent->AddForce((GrappleDirection * 2000 * CharacterMovementComponent->Mass) * GrapplingSpeed);
		}
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
		if (CharacterMovementComponent)
		{
			CharacterMovementComponent->SetMovementMode(MOVE_Flying);
			if (Player)
			{
				Player->MovementMultiplier = FVector(0.5f);
			}
		}

		GrappleState = EGrappleState::GS_PULLING;
	}
}

void AGrappleGun::OnMouseRightClickReleased(AActor* Actor)
{
	if (GrappleState == EGrappleState::GS_PULLING)
	{
		GrappleState = EGrappleState::GS_ATTACHED;

		if (CharacterMovementComponent)
		{
			CharacterMovementComponent->SetMovementMode(MOVE_Walking);
			Player->MovementMultiplier = FVector(1.0f);
		}
	}
}
