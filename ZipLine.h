// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZipLine.generated.h"

UCLASS()
class UE5_TEST_API AZipLine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZipLine();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zipline")
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zipline")
	class UCapsuleComponent* Collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zipline")
	class USceneComponent* ZiplineStartPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zipline")
	class USceneComponent* ZiplineEndPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zipline")
	float ZipliningSpeed;

private:
	class AMainCharacter* Player;

	bool bPlayerIsOnZipline;

	float ConstantZiplineSpeed;

	FVector ZiplineDirection;
	double NormalizedZiplineStartToEndDistance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnInteractionButtonPressed(AActor* player);

	void AttachPlayerToZipline();
	void SetPlayerMovementFlying(bool bIsFlying);
};
