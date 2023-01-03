// Fill out your copyright notice in the Description page of Project Settings.


#include "CollidingPawn.h"

#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "CollidingPawnMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#include "FloatingActor.h"

// Sets default values
ACollidingPawn::ACollidingPawn()
	: bHasPickupItemInHand(false), OverlappedPickupItem(nullptr)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComponent);
	Collision->SetGenerateOverlapEvents(true);
	Collision->InitSphereRadius(60.0f);
	

	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisual->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		SphereVisual->SetWorldScale3D(FVector(0.8f));
	}

	RevolvingSphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RevolvingSphere"));
	RevolvingSphere->SetupAttachment(RootComponent);
	if (SphereVisualAsset.Succeeded())
	{
		RevolvingSphere->SetStaticMesh(SphereVisualAsset.Object);
		RevolvingSphere->SetRelativeLocation(FVector(0.0f, 140.0f, 140.0f));
	}

	OurParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	OurParticleSystem->SetupAttachment(SphereVisual);
	OurParticleSystem->bAutoActivate = false;
	OurParticleSystem->SetRelativeLocation(FVector(-20.0f, 0.0f, 20.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	if (ParticleAsset.Succeeded())
	{
		OurParticleSystem->SetTemplate(ParticleAsset.Object);
	}

	USpringArmComponent* SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeRotation(FRotator(5.0f, 0.0f, 0.0f));

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	OurMovementComponent = CreateDefaultSubobject<UCollidingPawnMovementComponent>(TEXT("CustomMovementComponent"));
	OurMovementComponent->UpdatedComponent = RootComponent;
}

// Called when the game starts or when spawned
void ACollidingPawn::BeginPlay()
{
	Super::BeginPlay();
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ACollidingPawn::BeginOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ACollidingPawn::EndOverlap);
}

// Called every frame
void ACollidingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector Test = RevolvingSphere->GetRelativeLocation();

	const FVector NewLocation = Test.RotateAngleAxis(1.0f, FVector(0.0f, 0.0f, 1.0f));
	RevolvingSphere->SetRelativeLocation(NewLocation);
}

// Called to bind functionality to input
void ACollidingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("ParticleToggle", IE_Pressed, this, &ACollidingPawn::Jump);
	PlayerInputComponent->BindAction("TogglePickup", IE_Pressed, this, &ACollidingPawn::TogglePickupItem);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACollidingPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACollidingPawn::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ACollidingPawn::Turn);

}

UPawnMovementComponent* ACollidingPawn::GetMovementComponent() const
{
	return OurMovementComponent;
}

void ACollidingPawn::MoveForward(float AxisValue)
{
	if (OurMovementComponent && OurMovementComponent->UpdatedComponent == RootComponent)
	{
		OurMovementComponent->AddInputVector(GetActorForwardVector() * AxisValue);
	}
}

void ACollidingPawn::MoveRight(float AxisValue)
{
	if (OurMovementComponent && OurMovementComponent->UpdatedComponent == RootComponent)
	{
		OurMovementComponent->AddInputVector(GetActorRightVector() * AxisValue);
	}
}

void ACollidingPawn::Turn(float AxisValue)
{
	if (OurMovementComponent && OurMovementComponent->UpdatedComponent == RootComponent)
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += AxisValue;
		SetActorRotation(NewRotation);
	}
}

void ACollidingPawn::ParticleToggle()
{
	if (OurParticleSystem && OurParticleSystem->Template)
	{
		OurParticleSystem->ToggleActive();
	}
}

void ACollidingPawn::Jump()
{
	if (OurMovementComponent && OurMovementComponent->UpdatedComponent == RootComponent)
	{
		OurMovementComponent->AddInputVector(GetActorUpVector() * 1.0f);
	}
}

void ACollidingPawn::TogglePickupItem()
{
	if (bHasPickupItemInHand)
	{
		UE_LOG(LogTemp, Warning, TEXT("Detaching"));

		bHasPickupItemInHand = false;
		PickupItemInHand->bIsPickedUp = false;

		FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
		PickupItemInHand->DetachFromActor(DetachmentTransformRules);

		PickupItemInHand->SetActorEnableCollision(true);
		PickupItemInHand = nullptr;
	}
	else
	{
		if (!OverlappedPickupItem)
			return;

		PickupItemInHand = OverlappedPickupItem;
		bHasPickupItemInHand = true;
		PickupItemInHand->bIsPickedUp = true;

		FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
		PickupItemInHand->AttachToActor(this, AttachmentTransformRules);

		PickupItemInHand->SetActorEnableCollision(false);
	}
}

void ACollidingPawn::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFloatingActor* CollidingPawn = Cast<AFloatingActor>(OtherActor);

	if (CollidingPawn)
	{
		OverlappedPickupItem = CollidingPawn;
	}
}

void ACollidingPawn::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappedPickupItem = nullptr;
}

void ACollidingPawn::SwitchLevel(FName LevelName)
{
	UWorld* World = GetWorld();
	if (!World)
		return;

	FString CurrentLevel = World->GetMapName();
	FName CurrentLevelName(*CurrentLevel);

	if (CurrentLevelName != LevelName)
	{
		UGameplayStatics::OpenLevel(World, LevelName);
	}

}
