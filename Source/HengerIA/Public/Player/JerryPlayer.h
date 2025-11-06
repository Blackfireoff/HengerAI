// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Jerry.h"
#include "JerryPlayer.generated.h"

/**
 * 
 */
class UInputAction;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAimStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAimEnded);

UCLASS()
class HENGERIA_API AJerryPlayer : public AJerry
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* AimAction;

	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* BoostAction;

	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* ShootAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	float MaxStandardWalkSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	float MaxAimingWalkSpeed = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	float MaxBoostWalkSpeed = 800.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aiming")
	float DefaultArmLength = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Aiming")
	float AimingArmLength = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Aiming")
	float ZoomInterpSpeed = 10.0f;

	bool bIsAiming = false;

	bool bIsBoosting = false;

public:
	AJerryPlayer();

	/* Delegates*/
	FOnAimStarted OnAimStarted;
	FOnAimEnded OnAimEnded;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	void MoveInput(const FInputActionValue& Value);

	void JumpInputStart();

	void JumpInputEnd();

	void LookInput(const FInputActionValue& Value);

	void AimInputStart();

	void AimInputEnd();

	void BoostInputStart();

	void BoostInputEnd();

	virtual void ShootInput() override;

	void ResetHealth();

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
};
