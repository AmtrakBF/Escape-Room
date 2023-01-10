// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ladder.generated.h"

UCLASS()
class UE5_TEST_API ALadder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALadder();

	UPROPERTY(EditAnywhere);
	class UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere, Category = "Collision")
	class UBoxComponent* Collision;

	UPROPERTY(EditAnywhere, Category = "Collision")
	class UBoxComponent* LadderBottomCollision;

	UPROPERTY(EditAnywhere, Category = "Ladder")
	class UArrowComponent* LadderDirection;

	UPROPERTY(EditAnywhere, Category = "Ladder")
	float LadderSpeed;

private:
	class AMainCharacter* Player;

	class UCharacterMovementComponent* CharacterMovement;

	float MovementAxis;
	bool bIsOnGround;

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
	void OnLadderOverlapBottom(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OffLadderOverlapBottom(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
