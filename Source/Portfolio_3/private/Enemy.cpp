/*
* Writer : seo jae ung
*
* Function
*  - 적 캐릭터 클래스를 정의합니다. 
*  - 이 적은 특정 지점에 무기 공격 충돌 상자(Attack Collision)를 가지며, 이 충돌 상자가 다른 캐릭터와 겹치게 되면 일정한 애니메이션을 재생하면서 플레이어에게 피해를 입힙니다.
*  - 또한, 적은 움직임을 설정하고 특정 애니메이션을 플레이하는 데 사용되는 몽타주(Animation Montage)를 가집니다.
*
* UpdateRate : 2024 - 02 - 15
*/

#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <Components/BoxComponent.h>
#include <DrawDebugHelpers.h>
#include <TimerManager.h>

#include <PlayerCharacter.h>
#include <Projectile.h>

AEnemy::AEnemy() 
{
	PrimaryActorTick.bCanEverTick = true;

	windmillCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WindmillCollision"));
	windmillCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	windmillCollision->SetBoxExtent(FVector(300.0f, 300.0f, 10.0f));

	windmillCollision->AttachTo(GetMesh(), "WindmillSocket");

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (enemyName.ToString() == "Boss") {
		GetWorldTimerManager().SetTimer(
			bulletTimerHandle,
			this,
			&AEnemy::BulletSpawn,
			0.5f,
			true);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::EnemyAttack()
{
	PlayAnimMontage(montage);

	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);
	bool bResult = GetWorld()->SweepSingleByChannel(
		HitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * 200.0f,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(50.0f),
		Params);

	FVector TraceVec = GetActorForwardVector() * AttackRange;
	FVector Center = GetActorLocation() + TraceVec * 0.5f;
	float HalfHeight = AttackRange * 0.5f + AttackRadius;
	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	float DebugLifeTime = 1.0f;

	DrawDebugCapsule(GetWorld(),
		Center,
		HalfHeight,
		AttackRadius,
		CapsuleRot,
		DrawColor,
		false,
		DebugLifeTime);


	if (bResult) {
		if (HitResult.Actor.IsValid() && HitResult.Actor->IsA(APlayerCharacter::StaticClass())) {
			AWeapon* currentweapon = currentWeapon;
			APlayerCharacter* hit_actor = Cast<APlayerCharacter>(HitResult.Actor);
			if (hit_actor->health - attackDamage < 0.0f) {
				hit_actor->PlayAnimMontage(hit_actor->Death_AnimMontage, 1.0f);
				hit_actor->SetActorEnableCollision(false);
				hit_actor->Die();
			}
			else
			{
				hit_actor->health -= attackDamage;
			}
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Attack"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Damage is : %f"), hit_actor->health));
		}
	}
}

UAnimMontage* AEnemy::get_montage() const
{
	return montage;
}

void AEnemy::BossFunction80()
{
	if (enemyName.ToString() == "Boss") {
		if (health / maxHealth <= 0.8f) {
			SetActorScale3D(FVector(1.5f, 1.5f, 1.5f));
			maxHealth += 200.0f;
			health += 150.0f;
			walkSpeed += 100.0f;
			attackDamage += 1.0f;

			// Boss Pattern 1 -> BP_Enemy Spawn
			if (enemySpawnsClass != nullptr) {
				for (int i = 0; i < spawnEnemyNum; i++)
				{
					FDateTime Now = FDateTime::Now();
					int32 Seed = Now.GetSecond() + Now.GetMillisecond();
					FRandomStream RandomStream(Seed);
					int32 RandomNumber = RandomStream.RandRange(200, 400);

					FVector vpos = this->GetActorLocation();
					FVector spawnLocation = FVector(vpos.X - RandomNumber, vpos.Y - RandomNumber, vpos.Z);

					FActorSpawnParameters params;
					params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					GetWorld()->SpawnActor<AEnemy>(enemySpawnsClass, spawnLocation, GetActorRotation(), params);
				}
			}

			montage = bossMontage80;
		}
	}
}

void AEnemy::BossFunction50()
{
	if (enemyName.ToString() == "Boss") {
		if (health / maxHealth <= 0.5f) {
			SetActorScale3D(FVector(1.8f, 1.8f, 1.8f));
			maxHealth += 300.0f;
			health += 150.0f;
			walkSpeed += 100.0f;
			attackDamage += 1.0f;

			// Boss Parttern 2 -> Windmill
			GetWorldTimerManager().SetTimer(
				windmillTimeHandle,
				this,
				&AEnemy::WindmillCollisionEnable,
				1.0f,
				true);

			montage = bossMontage50;
		}
	}
}

void AEnemy::BossFunction30()
{
	if (enemyName.ToString() == "Boss") {
		if (health / maxHealth <= 0.3f) {
			SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
			maxHealth += 400.0f;
			health += 120.0f;
			walkSpeed += 100.0f;
			attackDamage += 1.0f;
			
			// Boss Parttern 3 -> Bullet
			GetWorldTimerManager().SetTimer(
				bulletTimerHandle,
				this,
				&AEnemy::BulletSpawn,
				0.5f,
				true);

			montage = bossMontage30;
		}
	}
}

void AEnemy::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (OtherActor->IsA(APlayerCharacter::StaticClass())) {
		UGameplayStatics::ApplyDamage(OtherActor, attackDamage, NULL, this, UDamageType::StaticClass());
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "ApplyDamage!");
	}
}

void AEnemy::WindmillCollisionEnable()
{
	--duringWindmill;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("CollisionEnable"));
	windmillCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if (duringWindmill < 1) {
		GetWorldTimerManager().ClearTimer(windmillTimeHandle);
	}
	windmillCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::BulletSpawn()
{
	if (bulletClass != nullptr) {
		--duringBulletSpawn;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Spawn Bullet"));

		FVector location = GetActorLocation();
		location.Z += 200.0f;

		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		GetWorld()->SpawnActor<AProjectile>(bulletClass, location, GetActorRotation(), params);

		if (duringBulletSpawn < 1) {
			GetWorldTimerManager().ClearTimer(bulletTimerHandle);
		}

	}
}