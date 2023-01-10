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
	: LadderSpeed(100.0f), LadderForce(1.0f), LadderBottomDistance(40.0f), LadderTopDistance(40.0f), Player(nullptr), CharacterMovement(nullptr), MovementAxis(0.0f), bIsOnGround(false), DistanceFromTopToCenter(0.0f), DistanceFromBottomToCenter(0.0f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;

	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	Collision->SetupAttachment(RootComponent);

	LadderDirection = CreateDefaultSubobject<UArrowComponent>(TEXT("LadderDirection"));
	LadderDirection->SetupAttachment(RootComponent);

	LadderTop = CreateDefaultSubobject<USceneComponent>(TEXT("LadderTop"));
	LadderTop->SetupAttachment(RootComponent);

	LadderBottom = CreateDefaultSubobject<USceneComponent>(TEXT("LadderBottom"));
	LadderBottom->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ALadder::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ALadder::BeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ALadder::EndOverlap);

	DistanceFromTopToCenter = FVector::Distance(GetActorLocation(), LadderTop->GetComponentLocation());
	DistanceFromBottomToCenter = FVector::Distance(GetActorLocation(), LadderBottom->GetComponentLocation());
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

	if (MovementAxis == 0.0f)
	{
		CharacterMovement->Velocity = FVector(0.0f);
		return;
	}

	float PlayerDistance = FVector::Distance(Player->GetActorLocation(), GetActorLocation());

	if (abs(DistanceFromTopToCenter - PlayerDistance) < (LadderTopDistance - (CharacterMovement->CrouchedHalfHeight * Player->bIsCrouched)) || abs(DistanceFromBottomToCenter - PlayerDistance) < LadderBottomDistance)
	{
		Player->MovementMultiplier = FVector(1.0f);
		if (CharacterMovement->Velocity.Z < LadderSpeed && CharacterMovement->Velocity.Z > -LadderSpeed)
		{
			CharacterMovement->AddForce((Player->GetActorUpVector() + ((LadderDirection->GetForwardVector() * MovementAxis) / 2.5)) * (CharacterMovement->Mass * -CharacterMovement->GetGravityZ()) * MovementAxis * (LadderForce * 1.1));
		}
		return;
	}

	Player->MovementMultiplier = (FVector(1.0f) - (LadderDirection->GetForwardVector().GetAbs())).GetAbs();
	if (CharacterMovement->Velocity.Z < LadderSpeed && CharacterMovement->Velocity.Z > -LadderSpeed)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Player->MovementMultiplier.ToString());
		CharacterMovement->AddForce((Player->GetActorUpVector() + ((LadderDirection->GetForwardVector() * MovementAxis))) * (CharacterMovement->Mass * -CharacterMovement->GetGravityZ()) * MovementAxis * (LadderForce * 1.1));
	}
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
}

void ALadder::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Player = Cast<AMainCharacter>(OtherActor);
	if (Player)
	{
		Player->MovementMultiplier = FVector(1.0f);
	}

	CharacterMovement = nullptr;
	Player = nullptr;
}
