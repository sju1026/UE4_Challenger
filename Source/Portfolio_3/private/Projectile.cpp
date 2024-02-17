// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Kismet/GameplayStatics.h>
#include <PlayerCharacter.h>

// Sets default values
AProjectile::AProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	collisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	collisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
	RootComponent = collisionComp;

	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	projectileMovement->UpdatedComponent = collisionComp;
	projectileMovement->InitialSpeed = 2000.0f;
	projectileMovement->MaxSpeed = 2000.0f;
	projectileMovement->bRotationFollowsVelocity = true;
	projectileMovement->bShouldBounce = false;
	InitialLifeSpan = 3.0f;
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
	FVector playerVector = player->GetActorLocation();
	FVector spawnPoint = GetActorLocation();

	projectileMovement->Velocity = playerVector - spawnPoint;
}

void AProjectile::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(APlayerCharacter::StaticClass())) {
		APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
		player->hitPlayerAsBullet++;
		
		if (player->health - bulletDamage <= 0) {
			player->Die();
		}
		player->health -= bulletDamage;

		Destroy();
	}
}
