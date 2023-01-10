// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractionKeyPressed, AActor*, Actor)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLeftMouseButtonPressed, AActor*, Actor)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRightMouseButtonPressed, AActor*, Actor)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRightMouseButtonReleased, AActor*, Actor)

struct TIventoryItem
{
	class ACollectableItem* Item;
	int count;
};

UCLASS()
class UE5_TEST_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	const int8 MAX_INVENTORY_SIZE = 32;

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UCapsuleComponent* CollisionCapsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Events")
	FOnInteractionKeyPressed OnInteractionKeyPressed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Events")
	FOnLeftMouseButtonPressed OnLeftMouseButtonPressed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Events")
	FOnRightMouseButtonPressed OnRightMouseButtonPressed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Events")
	FOnRightMouseButtonReleased OnRightMouseButtonReleased;

	FVector MovementMultiplier;

	bool bHasPickupItemInHand;
	bool bInterationKeyPressed;
	bool bJumpKeyPressed;

	ACollectableItem* PickupItemInHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<ACollectableItem*> OverlappedPickupItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<ACollectableItem*> Inventory;

private:
	float ForwardAxisValue;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void ToggleCrouch();

	void PlaceItemInHand();
	void DropItemInHand();
	void SwitchLevel(FName LevelName);

	void AddToInventory(class ACollectableItem* Item);
	void RemoveFromInventory(ACollectableItem* Item);
	bool IsItemInInventory(ACollectableItem* Item);

	float GetForwardMovementAxisValue();

	/* Returns index of closest overlapped item */
	uint32 GetClosestOverlappedItem();

	//! KeyBind Events

	void InteractionKeyPressed();
	inline void LeftMouseButtonPressed() { OnLeftMouseButtonPressed.Broadcast(this); };
	inline void RightMouseButtonPressed() { OnRightMouseButtonPressed.Broadcast(this); };
	inline void RightMouseButtonReleased() { OnRightMouseButtonReleased.Broadcast(this); };

	inline void InteractionKeyReleased() { bInterationKeyPressed = false; };
	inline void JumpKeyPressed() { bJumpKeyPressed = true; };
	inline void JumpKeyReleased() { bJumpKeyPressed = false; };
};
