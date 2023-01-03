// Fill out your copyright notice in the Description page of Project Settings.


#include "NextLevel.h"
#include "Components/BoxComponent.h"

#include "MainCharacter.h"

// Sets default values
ANextLevel::ANextLevel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TransitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("TransitionVolume"));
	RootComponent = TransitionVolume;
}

// Called when the game starts or when spawned
void ANextLevel::BeginPlay()
{
	Super::BeginPlay();
	TransitionVolume->OnComponentBeginOverlap.AddDynamic(this, &ANextLevel::BeginOverlap);

}

// Called every frame
void ANextLevel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ANextLevel::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
	
	if (MainCharacter)
		UE_LOG(LogTemp, Warning, TEXT("%s"), (MainCharacter->bHasPickupItemInHand ? TEXT("true") : TEXT("false")));

	if (!MainCharacter || LevelName.IsNone() || !MainCharacter->bHasPickupItemInHand)
		return;

	MainCharacter->SwitchLevel(LevelName);
}