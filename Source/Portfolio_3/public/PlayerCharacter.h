// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "PlayerAnim.h"
#include "PlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_3_API APlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	APlayerCharacter();

	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USpringArmComponent* cameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class UCameraComponent* followCam;

public:
	virtual void Tick(float deltaTime) override;

#pragma region AttackMontage
	UPROPERTY(EditDefaultsOnly, Category = MT)
		UAnimMontage* OneHand_Attack_Base;

	UPROPERTY(EditDefaultsOnly, Category = MT)
		UAnimMontage* OneHand_Attack_Q;

	UPROPERTY(EditDefaultsOnly, Category = MT)
		UAnimMontage* OneHand_Attack_E;

	UPROPERTY(EditDefaultsOnly, Category = MT)
		UAnimMontage* OneHand_Attack_C;

	UPROPERTY(EditDefaultsOnly, Category = MT)
		UAnimMontage* TwoHand_Attack_Base;

	UPROPERTY(EditDefaultsOnly, Category = MT)
		UAnimMontage* TwoHand_Attack_Q;

	UPROPERTY(EditDefaultsOnly, Category = MT)
		UAnimMontage* TwoHand_Attack_E;

	UPROPERTY(EditDefaultsOnly, Category = MT)
		UAnimMontage* TwoHand_Attack_C;
#pragma endregion

	bool isNone = true;
	bool isOneHand = false;
	bool isTwoHand = false;

	void OneHandUse();
	void TwoHandUse();

	void Attack_OneHand();
	void Attack_OneHand_Q();
	void Attack_OneHand_E();
	void Attack_OneHand_C();
	void Attack_OneHand_End();

	void AttackCheck();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* playerInput) override;


public:
	FTimerHandle playerTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		bool camYMove = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemBox)
		bool isBoxOpen = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
		bool isForest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
		bool isStone;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
		bool isWater;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = State)
		int32 hitPlayerAsBullet = 0;

private:
	void MoveForward(float value);

	void MoveRight(float value);

	void InputJump();

	UPROPERTY(EditAnywhere, Category = State)
		float sprintSpeed;

	void SprintEnter();
	void SprintEnd();

	UPROPERTY(EditAnywhere, Category = MT)
		UAnimMontage* dodgeAnim;

	UPROPERTY(EditAnywhere, Category = State)
		float dodgeDistance;

	bool dodgeable = true;
	UPROPERTY(EditAnywhere, Category = State)
		float dodgeTimer = 3.0f;
	void Dodging();

	void InputTurn(float value);

	void InputLookUp(float value);

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;

	// Boss Gimmic temp number
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class AEnemy* hit_enemy;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy", meta = (AllowPrivateAccess = "true"))
		bool bossHealth80 = false;

	UPROPERTY(VisibleAnywhere)
		bool bossHealth50 = false;

	UPROPERTY(VisibleAnywhere)
		bool bossHealth30 = false;

	int tempMontage = 0;
	int boss50MT = 0;

	bool skillQalbe = true;
	bool skillEable = true;
	bool skillCable = true;
	bool isStop = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Skill, meta = (AllowPrivateAccess = "true"))
		float skillTimer = 3.0f;

};
