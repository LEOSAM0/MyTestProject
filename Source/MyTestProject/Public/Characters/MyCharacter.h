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

class AMyBaseWeapon;
class UAnimMontage;

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

	// Equip weapon
	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
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


	// EquipWeapon-------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsEquipWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseWeaponSpawn")
	UAnimMontage* EquipAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BaseWeaponSpawn")
	UAnimMontage* UnEquipAnimMontage;
	
	FORCEINLINE bool IsEquippedWeapon() const { return bIsEquipWeapon; }

	UFUNCTION(BlueprintCallable)
	void SetIsEquippedWeapon(bool _IsEquippedWeapon);
	
	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	UInputAction* EquipAction;
	
	AActor* SpawnWeapon();
	void EquipWeapon();
	void UnEquipWeapon();
	void UnEquip();
	void Equip();
	void EquipComponents(AActor* Comp) const;
	void UnEquipComponents(AActor* Comp) const;
	void Spawning();
	
	UPROPERTY(EditAnywhere, Category = "BaseWeaponSpawn")
	TSubclassOf<AMyBaseWeapon> WeaponToSpawn;

	UPROPERTY(EditAnywhere, Category = "BaseWeaponSpawn")
	float AnimationDelay;

	bool bEquipUnEquiped = true;
	void EquipUnEquiped();
	//==================================================================
	// FIRE!
	UPROPERTY(EditAnywhere, Category= "EnhancedInput")
	UInputAction* FireAction;

	void Fire();
	AMyBaseWeapon* Weapon;
};

