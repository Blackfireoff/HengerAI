// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/JerryPlayer.h"

#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AI/JerryAI.h"

void AJerryPlayer::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = MaxStandardWalkSpeed;
	CameraBoom->TargetArmLength = DefaultArmLength;
	
	ResetHealth();
	ApplyTeamAsset(Team);
}

void AJerryPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TargetLength = bIsAiming ? AimingArmLength : DefaultArmLength;
	CameraBoom->TargetArmLength = FMath::FInterpTo(
		CameraBoom->TargetArmLength,
		TargetLength,
		DeltaTime,
		ZoomInterpSpeed
	);
}

// Called to bind functionality to input
void AJerryPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AJerryPlayer::MoveInput);

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AJerryPlayer::JumpInputStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AJerryPlayer::JumpInputEnd);

		// Looking
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AJerryPlayer::LookInput);

		// Aiming
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AJerryPlayer::AimInputStart);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AJerryPlayer::AimInputEnd);

		// Boosting
		EnhancedInputComponent->BindAction(BoostAction, ETriggerEvent::Started, this, &AJerryPlayer::BoostInputStart);
		EnhancedInputComponent->BindAction(BoostAction, ETriggerEvent::Completed, this, &AJerryPlayer::BoostInputEnd);

		// Shooting
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AJerryPlayer::ShootInput);
	}

}

AJerryPlayer::AJerryPlayer()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = DefaultArmLength;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AJerryPlayer::MoveInput(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	const FRotator Rotation = GetController()->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AJerryPlayer::JumpInputStart()
{
	Jump();
}

void AJerryPlayer::JumpInputEnd()
{
	StopJumping();
}

void AJerryPlayer::LookInput(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(-LookAxisVector.Y);
}

void AJerryPlayer::AimInputStart()
{
	GetCharacterMovement()->MaxWalkSpeed = MaxAimingWalkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
	bIsAiming = true;
	OnAimStarted.Broadcast();
}

void AJerryPlayer::AimInputEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = bIsBoosting ? MaxBoostWalkSpeed : MaxStandardWalkSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
	bIsAiming = false;
	OnAimEnded.Broadcast();
}

void AJerryPlayer::BoostInputStart()
{
	if (bIsAiming) return;
	GetCharacterMovement()->MaxWalkSpeed = MaxBoostWalkSpeed;
	bIsBoosting = true;
}

void AJerryPlayer::BoostInputEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = bIsAiming ? MaxAimingWalkSpeed : MaxStandardWalkSpeed;
	bIsBoosting = false;
}

void AJerryPlayer::ShootInput()
{
	UWorld* World = GetWorld();
	if (!World || !FollowCamera || !bIsAiming) return;
	
	Super::ShootInput();

	FVector CameraLocation = FollowCamera->GetComponentLocation();
	FVector CameraForward = FollowCamera->GetForwardVector();
	float MaxTraceDistance = 10000.f;

	FVector CameraTraceEnd = CameraLocation + (CameraForward * MaxTraceDistance);

	FHitResult CameraHitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	
	FVector TargetLocation = CameraTraceEnd;
	if (World->LineTraceSingleByChannel(CameraHitResult, CameraLocation, CameraTraceEnd, ECC_Visibility, CollisionParams))
	{
		TargetLocation = CameraHitResult.ImpactPoint;
	}
	
	FVector MuzzleLoc = SK_Gun->GetSocketLocation(TEXT("Muzzle"));
	
	FVector MuzzleToTargetDirection = (TargetLocation - MuzzleLoc).GetSafeNormal();
	FVector MuzzleTraceEnd = MuzzleLoc + (MuzzleToTargetDirection * MaxTraceDistance);

	FHitResult MuzzleHitResult;
	
	if (World->LineTraceSingleByChannel(MuzzleHitResult, MuzzleLoc, MuzzleTraceEnd, ECC_Visibility, CollisionParams))
	{

		DrawDebugLine(World, MuzzleLoc, MuzzleHitResult.ImpactPoint, FColor::Green, false, 2.0f, 0, 1.f);

		if (AActor* HitActor = MuzzleHitResult.GetActor())
		{
			if (AJerryAI* JerryAI = Cast<AJerryAI>(HitActor))
			{
				if (JerryAI->Team != Team)
				{	
					JerryAI->TakeDamage(WeaponDamage);
				}
			}
		}
	}
}

void AJerryPlayer::ResetHealth()
{
	CurrentHealth = MaxHealth;
}

