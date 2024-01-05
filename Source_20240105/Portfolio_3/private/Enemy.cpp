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
#include <UI_InGame.h>
#include <Blueprint/UserWidget.h>

AEnemy::AEnemy() 
{
	weaponCollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("AttackCollision"));
	weaponCollisionBox->AttachTo(GetMesh(), "WeaponPoint");
	 
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	/*static ConstructorHelpers::FObjectFinder<UUserWidget>ui_obj(TEXT("WidgetBlueprint'/Game/_My/UI/WBP_GamePlayerUI.WBP_GamePlayerUI'"));
	if (ui_obj.Succeeded()) {
		ui = ui_obj.Object;
	}*/

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

	/*if (ui) {
		UUI_InGame* bossUI = Cast<UUI_InGame>(ui);
		bossUI->set_HUDbar_Boss_value_percent(health / maxHealth);
	}*/

	if (isDeath == true) {
		weaponCollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
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

void AEnemy::BossFunction80()
{
	if (enemyName.ToString() == "Boss") {
		if (health / maxHealth <= 0.8f) {
			SetActorScale3D(FVector(1.2f, 1.2f, 1.2f));
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
			SetActorScale3D(FVector(1.5f, 1.5f, 1.5f));
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