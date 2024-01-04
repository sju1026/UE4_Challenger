// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCharacter.h"

#include "Weapon.generated.h"

UCLASS()
class PORTFOLIO_3_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon(const class FObjectInitializer& ObjectInitializer);

	// =================== Inventory ==================
	void SetOwningPawn(ABaseCharacter* NewOwner);
	void AttachMeshToPawn();
	void OnEquip(const AWeapon* LastWeapon);
	void OnUnEqip();
	// =================== Inventory ==================

private:
	UPROPERTY(VisibleDefaultsOnly, Category = Weapon)
		USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Weapon)
		class UBoxComponent* WeaponCollision;

	int tempMontage;

	int tempboss80;

protected:
	class ABaseCharacter* my;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon State")
		float weaponDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon State")
		float damagePlus = 0.0f;

};
