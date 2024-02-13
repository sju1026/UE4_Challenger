// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PORTFOLIO_3_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		FName CharacterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
		float walkSpeed;

#pragma region Inventory
	//============================================= Weapon =====================================================
	USkeletalMeshComponent* GetSpesificPawnMesh()const;

	FName GetWeaponAttachPoint() const;

	void EquipWeapon(class AWeapon* weapon);

	void OnChangeWeapon();

protected:

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		FName WeaponAttachPoint;

	TArray<class AWeapon*>inventory;

	void AddWeapon(class AWeapon* weapon);

	void SetCurrentWeapon(class AWeapon* newWeapon, class AWeapon* lastWeapon);

	void SpawnDefaultInventory();

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
		TArray<TSubclassOf<class AWeapon>>defaultInventoryClasses;

public:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
		class AWeapon* currentWeapon;
	//============================================= Weapon =====================================================
#pragma endregion

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)override;

protected:

	virtual void OnHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator, class AActor* DamageCauser);

	void OnDieAnimationEnd();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
		bool isDeath = false;

	UPROPERTY(EditDefaultsOnly, Category = MT)
		UAnimMontage* Death_AnimMontage;

	void Die();

	bool isDuringAttack = false;

	FTimerHandle deathTimer;
};
