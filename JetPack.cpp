// Fill out your copyright notice in the Description page of Project Settings.


#include "JetPack.h"
#include "MainCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AJetPack::AJetPack()
	: Fuel(50.0f), FuelBurntPerTick(0.07f), Thrust(7.0f)
{

}

void AJetPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsPickedUp && Player && !bIsOutOfFuel)
	{
		if (Player->bJumpKeyPressed)
		{
			Ignite();
		}
		else if (!Player->bJumpKeyPressed && CharacterMovementComponent && CharacterMovementComponent->MovementMode == EMovementMode::MOVE_Flying)
		{
			CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Falling);
			Player->MovementMultiplier = FVector(1.0f, 1.0f, 0.0f);
		}
	}
}

void AJetPack::Ignite()
{
	if (!Player)
		return;

	if (Fuel < 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Out Of Fuel!"));
		bIsOutOfFuel = true;

		if (CharacterMovementComponent)
		{
			CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Falling);
			Player->MovementMultiplier = FVector(1.0f, 1.0f, 0.0f);
		}
		return;
	}

	CharacterMovementComponent = Cast<UCharacterMovementComponent>(Player->GetMovementComponent());

	if (CharacterMovementComponent)
	{
		CharacterMovementComponent->SetMovementMode(EMovementMode::MOVE_Flying);
		CharacterMovementComponent->AddInputVector(FVector(0.0f, 0.0f, Thrust));
		Player->MovementMultiplier = FVector(15.0f, 15.0f, 0.0f);
		Fuel -= FuelBurntPerTick;
	}
}