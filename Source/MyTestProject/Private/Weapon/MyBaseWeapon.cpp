// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/MyBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "components/ArrowComponent.h"
#include "Bullet/MyBullet.h"


// Sets default values
AMyBaseWeapon::AMyBaseWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());

	Arrow = CreateDefaultSubobject<UArrowComponent>("ProjectileArrow");
	Arrow->SetupAttachment(Mesh);

}

// Called when the game starts or when spawned
void AMyBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyBaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyBaseWeapon::SpawnBullet()
{
		FActorSpawnParameters SpawnParameters;
				
		FTransform SpawnTransform = Arrow->GetComponentTransform();
		GetWorld()->SpawnActor<AMyBullet>(MyBulletClass, SpawnTransform, SpawnParameters);
}