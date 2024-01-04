/*
* Writer : seo jae ung
*
* Function
* - 무기 객체를 구현하여 무기 메시와 데미지를 처리합니다.
* - 무기 메시와 데미지 설정 후, 무기와 적 캐릭터 간의 충돌을 감지하여 데미지를 적용합니다.
* - 최대 데미지 값과 무기를 소유하는 캐릭터의 상호작용 및 장착/해제 기능도 구현되어 있습니다.
*
* UpdateRate : 2023 - 12 - 10
*/

#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "Engine.h"
#include <Enemy.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AWeapon::AWeapon(const class FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = WeaponMesh;

	WeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollision"));
	WeaponCollision->SetBoxExtent(FVector(5.0f, 5.0f, 5.0f));
	WeaponCollision->AttachTo(WeaponMesh, "DamageSocket");

	weaponDamage += damagePlus;
	tempboss80 = 0;
	tempMontage = 0;
}

// ==================== Inventory ====================
void AWeapon::SetOwningPawn(ABaseCharacter* NewOwner)
{
	if (my != NewOwner) {
		my = NewOwner;
	}
}

void AWeapon::AttachMeshToPawn()
{
	if (my)
	{
		USkeletalMeshComponent* PawnMesh = my->GetSpesificPawnMesh();
		FName AttachPoint = my->GetWeaponAttachPoint();
		WeaponMesh->AttachTo(PawnMesh, AttachPoint);
	}
}

void AWeapon::OnEquip(const AWeapon* LastWeapon)
{
	AttachMeshToPawn();
	WeaponMesh->SetHiddenInGame(false);
}

void AWeapon::OnUnEqip()
{
	WeaponMesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	WeaponMesh->SetHiddenInGame(true);
}
// ==================== Inventory ====================

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (my) {
		if (OtherActor->IsA(AEnemy::StaticClass()) && my->isDuringAttack) { // What to modify later
			AEnemy* boss = Cast<AEnemy>(OtherActor);
			UGameplayStatics::ApplyDamage(OtherActor, weaponDamage, NULL, this, UDamageType::StaticClass());
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "ApplyDamage!");
			if (boss->enemyName.ToString() == "Boss" && tempboss80 == 0) {
				boss->BossFunction();
				if (tempMontage == 0) {
					boss->PlayAnimMontage(boss->start80, 1.0f);
					tempMontage++;
				}
				tempboss80++;
			}
		}
	}
}

