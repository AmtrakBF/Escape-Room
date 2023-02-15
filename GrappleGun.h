// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableItem.h"
#include "GrappleGun.generated.h"

/**
 * 
 */

UENUM()
enum class EGrappleState : uint8
{
	GS_ATTACHED = 0,
	GS_DETACHED,
	GS_PULLING
};

UCLASS()
class UE5_TEST_API AGrappleGun : public ACollectableItem
{
	GENERATED_BODY()

public:
	AGrappleGun();

	EGrappleState GrappleState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grappling")
	class UCableComponent* Grapple;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grappling")
	float MaxGrappleDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grappling")
	float GrapplingSpeed;

private:
	FVector GrappleLocation;
	FVector GrappleDirection;

	float GrappleLength;
	float PlayerVelocity;

	class UCharacterMovementComponent* CharacterMovementComponent;

public:
	virtual void Tick(float DeltaTime) override;
	void TogglePickupItem() override;

	UFUNCTION()
	void OnMouseLeftClickPressed(AActor* Actor);
	UFUNCTION()
	void OnMouseRightClickPressed(AActor* Actor);
	UFUNCTION()
	void OnMouseRightClickReleased(AActor* Actor);

private:
	void AttachGrapple();
	void DetachGrapple();
	
};
