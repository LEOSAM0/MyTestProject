// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTestProject/Public/Characters/MyCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MyTestProject/Public/Weapon/MyBaseWeapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Components/ArrowComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/MyBaseWeapon.h"

// Sets default values
AMyCharacter::AMyCharacter() :
	WalkSpeed(400.f),
	RunSpeed(600.f)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	//SpringArm->TargetOffset = FVector(0, 110, 90);
	SpringArm->bInheritRoll = false;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 5.f;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->CameraRotationLagSpeed = 5.f;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	Camera->bUsePawnControlRotation = false;
	
	GetCharacterMovement()->JumpZVelocity = 300.f;
	GetCharacterMovement()->AirControl = 0.1f;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	AnimationDelay = 0.7f;

}

//TODO сменить положение винтовки с "BackWeapon" на "Weapon" с анимацией (на  блюпринтах уже реализовано без анимации)

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add input mapping content
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// Get local player subsystem
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Add input context
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}
	//UGameplayStatics::GetActorOfClass()
	
	Spawning();
	
}

void AMyCharacter::Move(const FInputActionValue& InputValue)
{
	FVector2d InputVector = InputValue.Get<FVector2d>();

	if (IsValid(Controller))
	{
		// Get forward direction
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);

		// Rotation matrix
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add movement input
		AddMovementInput(ForwardDirection, InputVector.Y);
		AddMovementInput(RightDirection, InputVector.X);
	}
}

void AMyCharacter::Look(const FInputActionValue& InputValue)
{
	FVector2d InputVector = InputValue.Get<FVector2d>();
	
	if (IsValid(Controller))
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}
}

void AMyCharacter::Jump(const FInputActionValue& InputValue)
{
	Super::Jump();
	
	ACharacter::Jump();

	/* но можно и так...но так длинее, МАМА, НЕ ГОРЮЙ!!!
	if (GetCharacterMovement()->IsMovingOnGround())
	{
		FVector ForwardVelocity = GetVelocity();
		ForwardVelocity.Z = 0.f;

		float JumpVerticalVelocity = GetCharacterMovement()->JumpZVelocity;

		FVector JumpVelocity = ForwardVelocity + FVector(0, 0, JumpVerticalVelocity);

		LaunchCharacter(JumpVelocity, true, true);
	}
	*/
}

void AMyCharacter::Running()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void AMyCharacter::StopRunning()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

/*-----------------------------------------------------------------------------------------------------------
                                                EQUIP WEAPON
// Equip Weapon */
AActor* AMyCharacter::SpawnWeapon()
{
	FActorSpawnParameters SpawnParameters;
	//SpawnParameters.Instigator = this;
	FTransform SpawnTransform;
	FVector SpawnVector = SpawnTransform.GetLocation();
	AActor* WeaponSpawned = GetWorld()->SpawnActor<AMyBaseWeapon>(WeaponToSpawn, SpawnVector, FRotator::ZeroRotator, SpawnParameters);
	return WeaponSpawned;
}

void AMyCharacter::Spawning()
{
	AActor* WeaponSpawned = SpawnWeapon();
	if (WeaponSpawned == nullptr) return;
	USceneComponent* WeaponParent = GetMesh();
	if (WeaponParent == nullptr) return;
	WeaponSpawned->AttachToComponent(WeaponParent, FAttachmentTransformRules::SnapToTargetIncludingScale, "BackWeapon");
}


// ____FLIP-FLOP____EquipUnEquiped_WEAPON
void AMyCharacter::EquipUnEquiped()
{
	if (bEquipUnEquiped)
	{
		Equip();
		bEquipUnEquiped = false;
	}
	else
	{
		UnEquip();
		bEquipUnEquiped = true;
	}
}

void AMyCharacter::EquipComponents(AActor* Comp) const
{
	USceneComponent* WeaponParent = GetMesh();
	if (WeaponParent == nullptr) return;
	Comp->AttachToComponent(WeaponParent, FAttachmentTransformRules::SnapToTargetIncludingScale, "Weapon");	
}

void AMyCharacter::EquipWeapon()
{
	AActor* EquipedWeapon = UGameplayStatics::GetActorOfClass(GetMesh(), WeaponToSpawn);
	if (EquipedWeapon == nullptr) return;
	
	EquipComponents(EquipedWeapon);
}

void AMyCharacter::Equip()
{
	GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, "Equip");

	SetIsEquippedWeapon(true);

	PlayAnimMontage(EquipAnimMontage);

	FTimerHandle BurnTimer;
	GetWorld()->GetTimerManager().SetTimer(BurnTimer, this, &AMyCharacter::EquipWeapon, AnimationDelay);
	
}

void AMyCharacter::UnEquipComponents(AActor* Comp) const
{
	USceneComponent* WeaponParent = GetMesh();
	if (WeaponParent == nullptr) return;
	Comp->AttachToComponent(WeaponParent, FAttachmentTransformRules::SnapToTargetIncludingScale, "BackWeapon");	
}

void AMyCharacter::UnEquipWeapon()
{
	AActor* EquipedWeapon = UGameplayStatics::GetActorOfClass(GetMesh(), WeaponToSpawn);
	if (EquipedWeapon == nullptr) return;
	UnEquipComponents(EquipedWeapon);	
}

void AMyCharacter::UnEquip()
{
	GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Red, "UnEquip");

	SetIsEquippedWeapon(false);
	
	PlayAnimMontage(EquipAnimMontage);

	FTimerHandle BurnTimer;
	GetWorld()->GetTimerManager().SetTimer(BurnTimer, this, &AMyCharacter::UnEquipWeapon, AnimationDelay);
}

void AMyCharacter::SetIsEquippedWeapon(bool _IsEquippedWeapon)
{
	bIsEquipWeapon = _IsEquippedWeapon;
}
//===========================================================================================================

// FIRE!

void AMyCharacter::Fire()
{
	if (bIsEquipWeapon)
	{
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Cyan, "Fire");
		
		if (Weapon)
		{
			GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Orange, "FireOPEN");
			Weapon->SpawnBullet();
		}
		
		GEngine->AddOnScreenDebugMessage(1, 2.f, FColor::Black, "FireCLOSE");
	}
}

//========================================================================

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Movement actions
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AMyCharacter::Jump);
		Input->BindAction(RunAction, ETriggerEvent::Started, this, &AMyCharacter::Running);
		Input->BindAction(RunAction, ETriggerEvent::Completed, this, &AMyCharacter::StopRunning);

		// Equip Weapon
		Input->BindAction(EquipAction, ETriggerEvent::Started, this, &AMyCharacter::EquipUnEquiped);

		// SpawnBullet
		Input->BindAction(FireAction, ETriggerEvent::Started, this, &AMyCharacter::Fire);
		
	}
}

