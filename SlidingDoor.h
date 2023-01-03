// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SlidingDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorOpen)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDoorClose)

UCLASS()
class UE5_TEST_API ASlidingDoor : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASlidingDoor();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UBoxComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	float DoorOpeningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	float DoorClosingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	FVector DoorOpeningTransform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	bool bIsLocked;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	bool bIsOpenedByButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	FName DoorKeyValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	class ACollectableItem* Key;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	float DoorCloseTimer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	bool bUseDoorCloseTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Events")
	FOnDoorOpen DoorOpen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Events")
	FOnDoorClose DoorClose;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	bool bIsOpening;
	bool bIsClosing;
	bool bIsOpen;

	FVector DoorClosingTransform;
	AActor* OverlappedActor;

	float ActiveDoorClosingTimer;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void Unlock(AActor* OtherActor);
	void Open(AActor* OtherActor);
	void Close();	
	void ToggleOpen(AActor* Player);

};

