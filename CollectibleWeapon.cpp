// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectibleWeapon.h"
#include "Components/CapsuleComponent.h"
#include "DestructibleItem.h"
#include "MainCharacter.h"

ACollectibleWeapon::ACollectibleWeapon()
	: WeaponType(EWeaponType::WT_NONE), Damage(10.0f), bIsAttacking(false)
{

}

void ACollectibleWeapon::Tick(float DeltaTime)
{

}

void ACollectibleWeapon::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ADestructibleItem* Item = Cast<ADestructibleItem>(OtherActor);

	if (Item && bIsAttacking)
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapping"));
		Item->ReductHealthPoints(Damage);
	}
}

void ACollectibleWeapon::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ACollectibleWeapon::Pickup()
{
	Super::Pickup();
	if (Player)
	{
		Player->OnLeftMouseButtonPressed.AddUniqueDynamic(this, &ACollectibleWeapon::Attack);
	}
	Collision->OnComponentBeginOverlap.AddUniqueDynamic(this, &ACollectibleWeapon::BeginOverlap);
}

void ACollectibleWeapon::Drop()
{
	Super::Drop();
	if (Player)
	{
		Player->OnLeftMouseButtonPressed.RemoveDynamic(this, &ACollectibleWeapon::Attack);
	}
	Collision->OnComponentBeginOverlap.RemoveDynamic(this, &ACollectibleWeapon::BeginOverlap);
}

bool ACollectibleWeapon::IsWeaponHitValid()
{
	return false;
}

void ACollectibleWeapon::Attack(AActor* MainCharacter)
{
	bIsAttacking = true;
	ActivateCollision();
	DeactivateCollision();
	bIsAttacking = false;
}

void ACollectibleWeapon::ActivateCollision()
{
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACollectibleWeapon::DeactivateCollision()
{
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
