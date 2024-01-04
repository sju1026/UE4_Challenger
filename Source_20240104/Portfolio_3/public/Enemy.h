// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CombatInterface.h"
#include <Animation/AnimMontage.h>

#include "Enemy.generated.h"


/**
 * 
 */
UCLASS()
class PORTFOLIO_3_API AEnemy : public ABaseCharacter, public ICombatInterface
{
	GENERATED_BODY()
	
public:
	AEnemy();

	virtual void NotifyActorBeginOverlap(AActor* otherActor) override;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime)override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
		class UBoxComponent* weaponCollisionBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
		FName enemyName;

	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
		class UBehaviorTree* botBehavior;*/

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
		float attackDamage = 10.0f;

	virtual int melee_attack_Implementation() override;

	UAnimMontage* get_montage() const;

	UFUNCTION()
		void BossFunction();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* start80;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* bossMontage80;

	float montageSpeed;

};
