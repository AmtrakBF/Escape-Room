// Fill out your copyright notice in the Description page of Project Settings.


#include "Ladder.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "MainCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ALadder::ALadder()
	: LadderSpeed(1.0f), Player(nullptr), CharacterMovement(nullptr), MovementAxis(0.0f), bIsOnGround(false)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	Collision->SetupAttachment(RootComponent);

	LadderBottomCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LadderCollision"));
	LadderBottomCollision->SetupAttachment(RootComponent);

	LadderDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("LadderDirection"));
	LadderDirection->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALadder::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ALadder::BeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ALadder::EndOverlap);

	LadderBottomCollision->OnComponentBeginOverlap.AddDynamic(this, &ALadder::OnLadderOverlapBottom);
	LadderBottomCollision->OnComponentEndOverlap.AddDynamic(this, &ALadder::OffLadderOverlapBottom);
}

// Called every frame
void ALadder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!Player)
		return;

	if (!CharacterMovement)
		return;

	MovementAxis = (Player->GetForwardMovementAxisValue() * Player->GetActorForwardVector().Dot(LadderDirection->GetForwardVector()));

	if (bIsOnGround && MovementAxis < 0)
	{
		return;
	}

	if (MovementAxis == 0.0f)
	{
		CharacterMovement->Velocity = FVector(0.0f);
		return;
	}

	Player->MovementMultiplier = (LadderDirection->GetForwardVector() - FVector(0.7f)).GetAbs();
	Player->AddMovementInput((Player->GetActorUpVector() + ((LadderDirection->GetForwardVector() * MovementAxis) / 2)) * MovementAxis);
}

void ALadder::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Player = Cast<AMainCharacter>(OtherActor);
	UCapsuleComponent* CollisionCapsule = Cast<UCapsuleComponent>(OtherComp);

	if (Player && CollisionCapsule == Player->CollisionCapsule)
	{
		Player = nullptr;
		return;
	}

	CharacterMovement = Cast<UCharacterMovementComponent>(Player->GetMovementComponent());
	if (CharacterMovement)
	{
		CharacterMovement->SetMovementMode(MOVE_Flying);
	}
}

void ALadder::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Player = Cast<AMainCharacter>(OtherActor);
	if (Player)
	{
		Player->MovementMultiplier = FVector(1.0f);
	}
	CharacterMovement = Cast<UCharacterMovementComponent>(Player->GetMovementComponent());
	if (CharacterMovement)
	{
		CharacterMovement->SetMovementMode(MOVE_Walking);
	}

	CharacterMovement = nullptr;
	Player = nullptr;
}

void ALadder::OnLadderOverlapBottom(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	bIsOnGround = true;
	if (Player)
	{
		Player->MovementMultiplier = FVector(1.0f);
	}
}

void ALadder::OffLadderOverlapBottom(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	bIsOnGround = false;
}

