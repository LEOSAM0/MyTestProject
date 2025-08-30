// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBaseWeapon.generated.h"

class USkeletalMeshComponent;
class UArrowComponent;
class USphereComponent;
class AMyBullet;

UCLASS()
class MYTESTPROJECT_API AMyBaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyBaseWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UArrowComponent* Arrow;

//Spawn the Bullet
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Bullet")
	TSubclassOf<AMyBullet> MyBulletClass;

	void SpawnBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
