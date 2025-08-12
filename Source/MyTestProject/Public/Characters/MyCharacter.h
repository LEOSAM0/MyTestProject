// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue; 

UCLASS()
class MYTESTPROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Movement
	void Move(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);
	void Jump(const FInputActionValue& InputValue);
	void Running();
	void StopRunning();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	UInputMappingContext* MappingContext;

	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	UInputAction* RunAction;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float WalkSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	float RunSpeed;
};

