// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "FloatingActor.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CollectableItem.h"

// Sets default values
AMainCharacter::AMainCharacter()
	: MovementMultiplier(1.0f), bHasPickupItemInHand(false), bInterationKeyPressed(false), bJumpKeyPressed(false), ForwardAxisValue(0.0f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlayerMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetGenerateOverlapEvents(false);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("PlayerSpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 600.0f;
	SpringArmComponent->SetRelativeRotation(FRotator(-15.f, 0.f, 0.f));
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	CollisionCapsule->SetupAttachment(RootComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionCapsule->OnComponentBeginOverlap.AddDynamic(this, &AMainCharacter::BeginOverlap);
	CollisionCapsule->OnComponentEndOverlap.AddDynamic(this, &AMainCharacter::EndOverlap);
}

static float Timer = 0.0f;
// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterationKeyPressed && PickupItemInHand)
	{
		Timer += DeltaTime;
		if (Timer > 0.5f)
		{
			DropItemInHand();
			Timer = 0.0f;
		}
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMainCharacter::ToggleCrouch);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::JumpKeyPressed);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::JumpKeyReleased);

	PlayerInputComponent->BindAction("TogglePickup", IE_Pressed, this, &AMainCharacter::InteractionKeyPressed);
	PlayerInputComponent->BindAction("TogglePickup", IE_Released, this, &AMainCharacter::InteractionKeyReleased); 
	PlayerInputComponent->BindAction("TogglePickup", IE_Pressed, this, &AMainCharacter::PlaceItemInHand);

	PlayerInputComponent->BindAction("LeftMouse", IE_Pressed, this, &AMainCharacter::LeftMouseButtonPressed);
	PlayerInputComponent->BindAction("RightMouse", IE_Pressed, this, &AMainCharacter::RightMouseButtonPressed);
	PlayerInputComponent->BindAction("RightMouse", IE_Released, this, &AMainCharacter::RightMouseButtonReleased);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
}

void AMainCharacter::MoveForward(float AxisValue)
{
	ForwardAxisValue = AxisValue;
	if ((Controller != nullptr) && (AxisValue != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * MovementMultiplier;
		AddMovementInput(Direction, AxisValue);
	}
}

void AMainCharacter::MoveRight(float AxisValue)
{
	if ((Controller != nullptr) && (AxisValue != 0.0f))
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * MovementMultiplier;
		AddMovementInput(Direction, AxisValue);
	}
}

void AMainCharacter::ToggleCrouch()
{
	if (!GetCharacterMovement()->IsCrouching())
	{
		Crouch();
	}
	else
	{
		UnCrouch();
	}
}

void AMainCharacter::PlaceItemInHand()
{
	Timer = 0.0f;

	if (bHasPickupItemInHand)
		return;

	uint32 OverlappedItemIndex = GetClosestOverlappedItem();
	if (OverlappedItemIndex == -1)
		return;

	bHasPickupItemInHand = true;
	PickupItemInHand = OverlappedPickupItems[OverlappedItemIndex];

	PickupItemInHand->Player = this;
	PickupItemInHand->TogglePickupItem();

	FAttachmentTransformRules AttachmentTransformRules(EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);
	PickupItemInHand->AttachToActor(this, AttachmentTransformRules);
}

void AMainCharacter::DropItemInHand()
{
	if (!bHasPickupItemInHand || !PickupItemInHand)
		return;

	bHasPickupItemInHand = false;
	PickupItemInHand->TogglePickupItem();

	FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
	PickupItemInHand->DetachFromActor(DetachmentTransformRules);

	PickupItemInHand->Player = nullptr;
	PickupItemInHand = nullptr;
}

void AMainCharacter::SwitchLevel(FName LevelName)
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

void AMainCharacter::AddToInventory(ACollectableItem* Item)
{
	if (!Item)
		return;

	//! Need implementation for notifying player of full inventory
	if (Inventory.Num() == MAX_INVENTORY_SIZE)
		return;

	Inventory.Add(Item);
}

void AMainCharacter::RemoveFromInventory(ACollectableItem* Item)
{
	if (!Item)
		return;

	Inventory.Remove(Item);
}

bool AMainCharacter::IsItemInInventory(ACollectableItem* Item)
{
	if (!Item)
		return false;

	for (auto& InventoryItem : Inventory)
	{
		if (InventoryItem->Name == Item->Name)
		{
			return true;
		}
	}

	return false;
}

float AMainCharacter::GetForwardMovementAxisValue()
{
	return ForwardAxisValue;
}

uint32 AMainCharacter::GetClosestOverlappedItem()
{
	if (OverlappedPickupItems.IsEmpty())
		return -1;

	uint32 closestItemIndex = 0;

	float closestDistance = GetDistanceTo(OverlappedPickupItems[0]);

	for (int x = 0; x < OverlappedPickupItems.Num(); x++)
	{
		float distance = GetDistanceTo(OverlappedPickupItems[x]);

		if (distance < closestDistance)
		{
			closestDistance = distance;
			closestItemIndex = x;
		}
	}

	return closestItemIndex;
}

void AMainCharacter::InteractionKeyPressed()
{
	OnInteractionKeyPressed.Broadcast(this);
	bInterationKeyPressed = true;
}

void AMainCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACollectableItem* Pickup = Cast<ACollectableItem>(OtherActor);

	if (Pickup)
	{
		OverlappedPickupItems.Add(Pickup);
	}
}

void AMainCharacter::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACollectableItem* Pickup = Cast<ACollectableItem>(OtherActor);

	if (Pickup)
	{
		OverlappedPickupItems.Remove(Pickup);
	}
}