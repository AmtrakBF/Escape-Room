// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestructibleItem.generated.h"

UCLASS()
class UE5_TEST_API ADestructibleItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructibleItem();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* StaticMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float HealthPoints;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ReductHealthPoints(float Amount);

private:

	void DestroyBox();
};
