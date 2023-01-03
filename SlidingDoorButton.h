// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlidingDoorButton.generated.h"

UCLASS()
class UE5_TEST_API ASlidingDoorButton : public AActor
{
	GENERATED_BODY()
	
private:
	TArray<class ASlidingDoor*> Doors;
	class AMainCharacter* Player;

	bool bIsButtonPressed;

public:	
	// Sets default values for this actor's properties
	ASlidingDoorButton();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UBoxComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	FName DoorKeyValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door")
	TSubclassOf<ASlidingDoor> ClassToFind;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UFUNCTION()
	void OnDoorClose();
	UFUNCTION()
	void OnDoorOpen();
	UFUNCTION()
	void ToggleDoorOpen(AActor* player);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
