// Fill out your copyright notice in the Description page of Project Settings.


#include "SlidingDoorButton.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

#include "SlidingDoor.h"
#include "MainCharacter.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASlidingDoorButton::ASlidingDoorButton()
	:bIsButtonPressed(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collison"));
	RootComponent = Collision;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ButtonMesh"));
	Mesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASlidingDoorButton::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ASlidingDoorButton::BeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ASlidingDoorButton::EndOverlap);

	TArray<AActor*> FoundActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);

	for (auto &actor : FoundActors)
	{
		auto door = Cast<ASlidingDoor>(actor);
		if (!door)
			continue;

		if (door->DoorKeyValue == DoorKeyValue)
		{
			Doors.Add(door);
			door->DoorClose.AddDynamic(this, &ASlidingDoorButton::OnDoorClose);
			door->DoorOpen.AddDynamic(this, &ASlidingDoorButton::OnDoorOpen);
		}
	}
}

void ASlidingDoorButton::OnDoorClose()
{
	bIsButtonPressed = false;
}

void ASlidingDoorButton::OnDoorOpen()
{
	bIsButtonPressed = true;
}

void ASlidingDoorButton::ToggleDoorOpen(AActor* player)
{
	bIsButtonPressed = !bIsButtonPressed;

	for (int x = 0; x < Doors.Num(); x++)
	{
		if (!Doors[x]->bIsOpenedByButton)
			continue;

		if (bIsButtonPressed)
		{
			Doors[x]->Open(Player);
		}
		else
		{
			Doors[x]->Close();
		}
	}
}

// Called every frame
void ASlidingDoorButton::Tick(float DeltaTime)
{
}

void ASlidingDoorButton::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Doors.IsEmpty())
		return;

	Player = Cast<AMainCharacter>(OtherActor);
	if (Player->IsValidLowLevel())
	{
		Player->OnInteractionKeyPressed.AddUniqueDynamic(this, &ASlidingDoorButton::ToggleDoorOpen);
	}
}

void ASlidingDoorButton::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Player)
	{
		Player->OnInteractionKeyPressed.RemoveDynamic(this, &ASlidingDoorButton::ToggleDoorOpen);
	}
	Player = nullptr;
}