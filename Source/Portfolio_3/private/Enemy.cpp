/*
* Writer : seo jae ung
*
* Function
*  - 적 캐릭터 클래스를 정의합니다. 
*  - 이 적은 특정 지점에 무기 공격 충돌 상자(Attack Collision)를 가지며, 이 충돌 상자가 다른 캐릭터와 겹치게 되면 일정한 애니메이션을 재생하면서 플레이어에게 피해를 입힙니다.
*  - 또한, 적은 움직임을 설정하고 특정 애니메이션을 플레이하는 데 사용되는 몽타주(Animation Montage)를 가집니다.
*
* UpdateRate : 2024 - 02 - 13
*/

#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <UObject/ConstructorHelpers.h>
#include <PlayerCharacter.h>
#include <DrawDebugHelpers.h>

AEnemy::AEnemy() 
{
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;
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

			/*UUI_InGame* bossUI = Cast<UUI_InGame>(ui);
			bossUI->HUD_BossHPbar_value->SetVisibility(ESlateVisibility::Visible);*/
			
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
			
			montage = bossMontage30;
		}
	}
}