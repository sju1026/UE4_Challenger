// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CombatInterface.h"
#include <Animation/AnimMontage.h>

#include "Enemy.generated.h"

UCLASS()
class PORTFOLIO_3_API AEnemy : public ABaseCharacter, public ICombatInterface
{
	GENERATED_BODY()
	
public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime)override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
		FName enemyName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = State)
		float attackDamage = 10.0f;

	void EnemyAttack();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Attack)
		float AttackRange = 200.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Attack)
		float AttackRadius = 50.0f;

	UAnimMontage* get_montage() const;

	UFUNCTION()
		void BossFunction80();

	UFUNCTION()
		void BossFunction50();

	UFUNCTION()
		void BossFunction30();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* start80;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* boss50;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* bossMontage80;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* bossMontage50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		UAnimMontage* bossMontage30;

	float montageSpeed;

	// Spawn Enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossPattern", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<AEnemy> enemySpawnsClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossPattern", meta = (AllowPrivateAccess = "true"))
		int32 spawnEnemyNum = 5;

	// Windmill
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossPattern", meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* windmillCollision;

	UFUNCTION()
		virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	// SpawnBullet
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossPattern", meta = (AllowPrivateAccess = "true"))
		TSubclassOf<class AProjectile> bulletClass;


	FTimerHandle windmillTimeHandle;
	int32 duringWindmill = 3;
	void WindmillCollisionEnable();

	FTimerHandle bulletTimerHandle;
	int32 duringBulletSpawn = 6;
	void BulletSpawn();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reward")
		TSubclassOf<AActor> nextLevelClass;


};
