/*
* Writer : seo jae ung
*
* Function
*  - 적 캐릭터 클래스를 정의합니다. 
*  - 이 적은 특정 지점에 무기 공격 충돌 상자(Attack Collision)를 가지며, 이 충돌 상자가 다른 캐릭터와 겹치게 되면 일정한 애니메이션을 재생하면서 플레이어에게 피해를 입힙니다.
*  - 또한, 적은 움직임을 설정하고 특정 애니메이션을 플레이하는 데 사용되는 몽타주(Animation Montage)를 가집니다.
*
* UpdateRate : 2024 - 01 - 04
*/

#include "Enemy.h"
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <UObject/ConstructorHelpers.h>
#include <Blueprint/UserWidget.h>
#include "Components/WidgetComponent.h"
#include <PlayerCharacter.h>

AEnemy::AEnemy() 
{
	weaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	weaponCollisionBox->AttachTo(GetMesh(), "WeaponPoint");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	health = maxHealth;
	montageSpeed = 1.0f;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::NotifyActorBeginOverlap(AActor* otherActor)
{
	Super::NotifyActorBeginOverlap(otherActor);

	if (otherActor->IsA(APlayerCharacter::StaticClass()) && PlayAnimMontage(montage) > 0.2f) {
		UGameplayStatics::ApplyDamage(otherActor, attackDamage, NULL, this, UDamageType::StaticClass());
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int AEnemy::melee_attack_Implementation()
{
	if (montage) {
		if (enemyName == "Enemy") {
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Melee Attack from Enemy"));
			PlayAnimMontage(montage, montageSpeed);
		}
	}

	return 0;
}

UAnimMontage* AEnemy::get_montage() const
{
	return montage;
}

void AEnemy::BossFunction()
{
	if (enemyName.ToString() == "Boss") {
		if (health / maxHealth <= 0.8f) {
			SetActorScale3D(FVector(2.0f, 2.0f, 2.0f));
			maxHealth = 200.0f;
			health = 150.0f;
			walkSpeed += 100.0f;
			attackDamage += 10.0f;

			montage = bossMontage80;
		}
	}
}
