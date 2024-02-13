/*
* Writer : seo jae ung
*
* Function
* - 코드는 기본적으로 캐릭터 클래스를 정의하며, 캐릭터의 기능을 초기화하고 관리합니다. 
* - 캐릭터의 체력, 무기 인벤토리, 무기 교체 및 피격 시 애니메이션, 사망 처리 등을 다룹니다. 
* - 각 함수는 캐릭터의 특정 동작에 대응하여 실행됩니다.
* 
* UpdateRate : 2024 - 02 - 13
*/


#include "BaseCharacter.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine.h"
#include "Weapon.h"
#include <TimerManager.h>

// Sets default values
ABaseCharacter::ABaseCharacter()
	:health(maxHealth)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	isDeath = false;
	isDuringAttack = false;
	maxHealth = 100.0f;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	health = maxHealth;
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USkeletalMeshComponent* ABaseCharacter::GetSpesificPawnMesh() const
{
	return GetMesh();
}

FName ABaseCharacter::GetWeaponAttachPoint() const
{
	return WeaponAttachPoint;
}

void ABaseCharacter::AddWeapon(AWeapon* Weapon)
{
	if (Weapon)
	{
		inventory.AddUnique(Weapon);
	}
}

void ABaseCharacter::SetCurrentWeapon(AWeapon* NewWeapon, AWeapon* LastWeapon)
{
	AWeapon* LocalLastWeapon = NULL;
	if (LastWeapon != NULL)
	{
		LocalLastWeapon = LastWeapon;
	}
	else if (NewWeapon != currentWeapon) {
		LocalLastWeapon = currentWeapon;
	}

	if (LocalLastWeapon) {
		LocalLastWeapon->OnUnEqip();
	}

	currentWeapon = NewWeapon;

	if (NewWeapon)
	{
		NewWeapon->SetOwningPawn(this);
		NewWeapon->OnEquip(LastWeapon);
	}
}

void ABaseCharacter::EquipWeapon(AWeapon* Weapon)
{
	if (Weapon)
	{
		SetCurrentWeapon(Weapon, currentWeapon);
	}
}

void ABaseCharacter::SpawnDefaultInventory()
{
	int32 NumWeaponClasses = defaultInventoryClasses.Num();

	/*if (DefaultInventoryClasses[0])
	{
		FActorSpawnParameters SpawnInfo;
		UWorld* WRLD = GetWorld();
		AMyTestWeapon* NewWeapon = WRLD->SpawnActor<AMyTestWeapon>(DefaultInventoryClasses[0], SpawnInfo);
		AddWeapon(NewWeapon);
	}*/

	for (int32 i = 0; i < NumWeaponClasses; i++)
	{
		if (defaultInventoryClasses[i]) {
			FActorSpawnParameters SpawnInfo;

			UWorld* WRLD = GetWorld();
			AWeapon* NewWeapon = WRLD->SpawnActor<AWeapon>(defaultInventoryClasses[i], SpawnInfo);
			AddWeapon(NewWeapon);
		}
	}

	if (inventory.Num() > 0)
	{
		EquipWeapon(inventory[0]);
	}

}

void ABaseCharacter::OnChangeWeapon()
{
	const int32 CurrentWeaponIndex = inventory.IndexOfByKey(currentWeapon);

	AWeapon* NextWeapon = inventory[(CurrentWeaponIndex + 1) % inventory.Num()];

	EquipWeapon(NextWeapon);

	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "change!");
}

#pragma region Hit

float ABaseCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float myGetDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	myGetDamage = FMath::Min(health, myGetDamage);

	if (myGetDamage > 0.f)
	{
		health -= myGetDamage;
	}

	if (health <= 0)
	{
		PlayAnimMontage(Death_AnimMontage, 1.0f);
		Die();
	}
	else
	{
		OnHit(myGetDamage, DamageEvent, EventInstigator ? EventInstigator->GetPawn() : NULL, DamageCauser);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HP is : %f"), health));
	}

	return myGetDamage;
}

void ABaseCharacter::OnHit(float DamageTaken, FDamageEvent const& DamageEvent, APawn* PawnInstigator, AActor* DamageCauser)
{
	if (DamageTaken > 0.f)
	{
		ApplyDamageMomentum(DamageTaken, DamageEvent, PawnInstigator, DamageCauser);
	}
}

void ABaseCharacter::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("DeathFunction!"));
	isDeath = true;

	health = FMath::Min(0.f, health);

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
	}

	DetachFromControllerPendingDestroy();

	float deathTime = PlayAnimMontage(Death_AnimMontage);

	GetWorldTimerManager().SetTimer(
		deathTimer,
		FTimerDelegate::CreateLambda([this]()->void {UE_LOG(LogTemp, Warning, TEXT("Death!")); this->Destroy(); }),
		deathTime,
		false);
}

void ABaseCharacter::OnDieAnimationEnd()
{
	Destroy();
}
#pragma endregion

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

