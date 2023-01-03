// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableItem.h"
#include "JetPack.generated.h"

/**
 * 
 */
UCLASS()
class UE5_TEST_API AJetPack : public ACollectableItem
{
	GENERATED_BODY()

public:
	AJetPack();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jetpack")
	float Fuel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jetpack")
	float FuelBurntPerTick;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Jetpack")
	float Thrust;

private:
	class UCharacterMovementComponent* CharacterMovementComponent;

	bool bIsOutOfFuel;

public:
	virtual void Tick(float DeltaTime) override;

	void Ignite();

};
