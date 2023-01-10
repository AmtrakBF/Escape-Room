// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollectableItem.h"
#include "CollectibleWeapon.generated.h"

UENUM()
enum class EWeaponType : uint8
{
	WT_NONE = 0,
	WT_MELEE,
	WT_RANGEDWEAPON_LIGHTWEIGHT,
	WT_RANGEDWEAPON_MIDWEIGHT,
	WT_RANGEDWEAPON_HEAVYWEIGHT
};

UCLASS()
class UE5_TEST_API ACollectibleWeapon : public ACollectableItem
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ACollectibleWeapon();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon Properties")
	float Damage;

private:

	bool bIsAttacking;

public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void Pickup() override;
	void Drop() override;

	bool IsWeaponHitValid();

	UFUNCTION()
	void Attack(AActor* MainCharacter);

	void ActivateCollision();
	void DeactivateCollision();
};
