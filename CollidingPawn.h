// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CollidingPawn.generated.h"

UCLASS()
class UE5_TEST_API ACollidingPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollidingPawn();

	bool bHasPickupItemInHand;

	class AFloatingActor* OverlappedPickupItem;
	AFloatingActor* PickupItemInHand;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void ParticleToggle();
	void Jump();

	void TogglePickupItem();

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SwitchLevel(FName LevelName);

	UPROPERTY()
	class UStaticMeshComponent* RevolvingSphere;

	UPROPERTY()
	class UParticleSystemComponent* OurParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UCollidingPawnMovementComponent* OurMovementComponent;

	UPROPERTY()
	class USphereComponent* Collision;

};
