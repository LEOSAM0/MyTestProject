// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "MyTestProject/Public/Characters/MyCharacter.h"

// Acts like Tick for animation and get speed
void UMyAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
	if (MyCharacter == nullptr)
	{
		MyCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
	}

	if (MyCharacter)
	{
		// Get speed of character from the velocity
		FVector Velocity = MyCharacter->GetVelocity();
		Velocity.Z = 0;
		Speed = Velocity.Size();

		bIsInAir = MyCharacter->GetCharacterMovement()->IsFalling();

		// Get offset yaw for player in blend spaces
		FRotator const AimRotation = MyCharacter->GetBaseAimRotation();
		FRotator const MovementRotation = UKismetMathLibrary::MakeRotFromX(MyCharacter->GetVelocity());

		Direction = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation, AimRotation).Yaw;
	}
}

// Acts like BeginPlay create reference to character
void UMyAnimInstance::NativeInitializeAnimation()
{
	MyCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
}
