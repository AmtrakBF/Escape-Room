// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlidingPlatform.generated.h"

UCLASS()
class UE5_TEST_API ASlidingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASlidingPlatform();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	class UStaticMeshComponent* PlatformMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MovementDelay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	float MovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector PlatformTransform;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bOscillateMovement;

private:

	float MovementTimer;

	FVector ClosedPosition;
	FVector OpenedPosition;

	bool bIsOpen;
	bool bIsOpening;
	bool bIsClosing;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
