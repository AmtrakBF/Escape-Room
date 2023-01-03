// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectableItem.generated.h"


UCLASS()
class UE5_TEST_API ACollectableItem : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ACollectableItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UCapsuleComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName Name;

	class AMainCharacter* Player;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bCanBePlacedInInventory;

	bool bPhysicsEnabled;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bIsPickedUp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void TogglePickupItem();
	virtual void Pickup();
	virtual void Drop();

};
