// Fill out your copyright notice in the Description page of Project Settings.


#include "SlidingDoor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "MainCharacter.h"
#include "CollectableItem.h"
#include "Engine/World.h"

// Sets default values
ASlidingDoor::ASlidingDoor()
	: DoorOpeningSpeed(2.0f), DoorClosingSpeed(2.0f), DoorOpeningTransform(1.0f), bIsLocked(false), bIsOpenedByButton(false), DoorCloseTimer(5.0f), bUseDoorCloseTimer(false), bIsOpening(false), bIsClosing(false),
	bIsOpen(false), ActiveDoorClosingTimer(0.0f)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collison"));
	RootComponent = Collision;

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorMesh"));
	DoorMesh->SetGenerateOverlapEvents(false);
	DoorMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASlidingDoor::BeginPlay()
{
	Super::BeginPlay();
	
	if (!bIsOpenedByButton)
	{
		Collision->OnComponentBeginOverlap.AddDynamic(this, &ASlidingDoor::BeginOverlap);
		Collision->OnComponentEndOverlap.AddDynamic(this, &ASlidingDoor::EndOverlap);
	}

	DoorClosingTransform = DoorMesh->GetRelativeLocation();
	DoorOpeningTransform += DoorMesh->GetRelativeLocation() + DoorOpeningTransform;
}

// Called every frame
void ASlidingDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsOpening)
	{
		DoorMesh->SetRelativeLocation(FMath::Lerp(DoorMesh->GetRelativeLocation(), DoorOpeningTransform, DoorOpeningSpeed * DeltaTime));

		if (FVector::PointsAreNear(DoorMesh->GetRelativeLocation(), DoorOpeningTransform, 1.0))
		{
			bIsOpening = false;
			bIsOpen = true;
			DoorOpen.Broadcast();
		}
	}

	if (bIsClosing)
	{
		DoorMesh->SetRelativeLocation(FMath::Lerp(DoorMesh->GetRelativeLocation(), DoorClosingTransform, DoorClosingSpeed * DeltaTime));

		if (FVector::PointsAreNear(DoorMesh->GetRelativeLocation(), DoorClosingTransform, 1.0))
		{
			bIsClosing = false;
			bIsOpen = false;
			DoorClose.Broadcast();
		}
	}

	if (bIsOpen && bUseDoorCloseTimer && !bIsClosing)
	{
		ActiveDoorClosingTimer += DeltaTime;
		if (ActiveDoorClosingTimer > DoorCloseTimer)
		{
			bIsClosing = true;
			ActiveDoorClosingTimer = 0.0f;
		}
	}
}

// Called to bind functionality to input
void ASlidingDoor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ASlidingDoor::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if ((OverlappedActor && !bIsOpenedByButton))
		return;

	OverlappedActor = OtherActor;
	ToggleOpen(OverlappedActor);
}

void ASlidingDoor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappedActor = nullptr;
}

void ASlidingDoor::Unlock(AActor* OtherActor)
{

	AMainCharacter* Player = Cast<AMainCharacter>(OtherActor);
	
	if (!Player || !Key)
		return;

	if (Player->IsItemInInventory(Key))
	{
		bIsLocked = false;
	}
	else
	{
		bIsLocked = true;
	}
}

void ASlidingDoor::Open(AActor* OtherActor)
{
	Unlock(OtherActor);

	if (bIsLocked)
		return;

	bIsOpening = true;
	bIsClosing = false;
}

void ASlidingDoor::Close()
{
	bIsOpening = false;
	bIsClosing = true;
}

void ASlidingDoor::ToggleOpen(AActor* Player)
{
	if (bIsOpen)
	{
		Close();
	}
	else
	{
		Open(Player);
	}
}
