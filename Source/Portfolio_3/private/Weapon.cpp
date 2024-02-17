/*
* Writer : seo jae ung
*
* Function
* - 무기 객체를 구현하여 무기 메시와 데미지를 처리합니다.
* - 무기 메시와 데미지 설정 후, 무기와 적 캐릭터 간의 충돌을 감지하여 데미지를 적용합니다.
* - 최대 데미지 값과 무기를 소유하는 캐릭터의 상호작용 및 장착/해제 기능도 구현되어 있습니다.
*
* UpdateRate : 2024 - 02 - 13
*/

#include "Weapon.h"
#include "Engine.h"
#include <Kismet/GameplayStatics.h>
#include <Components/PointLightComponent.h>
#include <Components/BoxComponent.h>
#include <Enemy.h>
#include <Projectile.h>


// Sets default values
AWeapon::AWeapon(const class FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("WeaponMesh"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = WeaponMesh;

	spotLight_OneHand = CreateDefaultSubobject<UPointLightComponent>(TEXT("OneHand_SpotLight"));
	spotLight_TwoHand = CreateDefaultSubobject<UPointLightComponent>(TEXT("TwoHand_SpotLight"));

	spotLight_OneHand->SetupAttachment(RootComponent);
	spotLight_TwoHand->SetupAttachment(RootComponent);

	spotLight_OneHand->SetRelativeLocation(FVector(0, 0, 0));
	spotLight_TwoHand->SetRelativeLocation(FVector(0, 0, 0));

	spotLight_OneHand->Intensity = 50000.0f;
	spotLight_OneHand->AttenuationRadius = 150.0f;

	spotLight_TwoHand->Intensity = 50000.0f;
	spotLight_TwoHand->AttenuationRadius = 150.0f;

	spotLight_OneHand->SetVisibility(false);
	spotLight_TwoHand->SetVisibility(false);

	weaponDamage += damagePlus;

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

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

