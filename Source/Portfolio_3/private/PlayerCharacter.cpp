/*
* Writer : seo jae ung
*
* Function
* - 플레이어 캐릭터 클래스로, 캐릭터의 이동, 점프, 대시, 카메라 회전, 공격 및 상호작용과 같은 기능을 구현합니다. 
* - 또한 무기 사용 및 전환, 애니메이션 실행과 관련된 기능을 포함합니다.
* - PrimaryActorTick을 통해 주기적인 Tick을 활성화하며, 입력에 따라 캐릭터의 행동을 제어합니다.
* - 상호작용을 통해 상자를 열거나 무기를 전환하며, 각종 동작을 애니메이션으로 구현하고 있습니다.
*
* UpdateRate : 2024 - 02 - 15
*/

#include "PlayerCharacter.h"
#pragma region EngineHeader

#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/PlayerController.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <UObject/ConstructorHelpers.h>
#include <Components/PointLightComponent.h>
#include <Components/CapsuleComponent.h>
#include "DrawDebugHelpers.h"
#include <Kismet/GameplayStatics.h>
#include <UObject/ConstructorHelpers.h>
#include <Blueprint/UserWidget.h>

#pragma endregion

#include <Portfolio_3GameModeBase.h>
#include <Weapon.h>
#include <ItemBox.h>
#include <Enemy.h>
#include <TimerManager.h>

APlayerCharacter::APlayerCharacter() {
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	cameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	cameraBoom->SetupAttachment(RootComponent);
	cameraBoom->TargetArmLength = 1000.0f;
	cameraBoom->bUsePawnControlRotation = true;

	followCam = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	followCam->SetupAttachment(cameraBoom, USpringArmComponent::SocketName);
	followCam->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;

	walkSpeed = 600.0f;
	dodgeDistance = 7000.0f;
	sprintSpeed = 300.0f;

	isForest = false;
	isStone = false;
	isWater = false;

	AttackRange = 200.0f;
	AttackRadius = 50.0f;
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SpawnDefaultInventory();
}

void APlayerCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	if (hitPlayerAsBullet == 5 && isStop == false) {
		isStop = false;

		GetCharacterMovement()->MaxWalkSpeed = 0;

		FTimerHandle stopTimerHandle;
		GetWorldTimerManager().SetTimer(
			stopTimerHandle,
			FTimerDelegate::CreateLambda([this]()->void {GetCharacterMovement()->MaxWalkSpeed = walkSpeed; }),
			3.0f,
			false);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* playerInput)
{
	check(playerInput);
	playerInput->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	playerInput->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	playerInput->BindAxis("Turn", this, &APlayerCharacter::InputTurn);
	playerInput->BindAxis("LookUp", this, &APlayerCharacter::InputLookUp);

	playerInput->BindAction("Jump",IE_Pressed, this, &APlayerCharacter::InputJump);
	playerInput->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::SprintEnter);
	playerInput->BindAction("Sprint", IE_Released, this, &APlayerCharacter::SprintEnd);
	playerInput->BindAction("Dodge", IE_Pressed, this, &APlayerCharacter::Dodging);

	playerInput->BindAction("Attack", IE_Pressed, this, &APlayerCharacter::Attack_OneHand);
	playerInput->BindAction("AttackComboQ", IE_Pressed, this, &APlayerCharacter::Attack_OneHand_Q);
	playerInput->BindAction("AttackComboE", IE_Pressed, this, &APlayerCharacter::Attack_OneHand_E);
	playerInput->BindAction("AttackComboC", IE_Pressed, this, &APlayerCharacter::Attack_OneHand_C);
	playerInput->BindAction("OneHand", IE_Pressed, this, &APlayerCharacter::OneHandUse);
	playerInput->BindAction("TwoHand", IE_Pressed, this, &APlayerCharacter::TwoHandUse);
}

#pragma region Movement

void APlayerCharacter::MoveForward(float value)
{
	if ((Controller != NULL) && (value != 0.0f) && !isDuringAttack) {
		const FRotator rot = Controller->GetControlRotation();
		const FRotator YawRot(0, rot.Yaw, 0);
		const FVector direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
		AddMovementInput(direction, value);
	}
}

void APlayerCharacter::MoveRight(float value)
{
	if ((Controller != NULL) && (value != 0.0f) && !isDuringAttack) {
		const FRotator rot = Controller->GetControlRotation();
		const FRotator YawRot(0, rot.Yaw, 0);
		const FVector direction = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);
		AddMovementInput(direction, value);
	}
}

void APlayerCharacter::InputJump()
{
	Jump();
}

void APlayerCharacter::SprintEnter()
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed + sprintSpeed;
}

void APlayerCharacter::SprintEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = walkSpeed;
}

void APlayerCharacter::Dodging()
{
	if (dodgeable) {
		dodgeable = false;
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("NoCollision"));
		PlayAnimMontage(dodgeAnim, 2.0f);

		const FVector forwardDir = this->GetActorRotation().Vector();
		LaunchCharacter(forwardDir * dodgeDistance, true, false);

		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("QueryAndPhysics"));

		FTimerHandle dodgetTimeHandle;
		GetWorldTimerManager().SetTimer(
			dodgetTimeHandle,
			FTimerDelegate::CreateLambda([this]()->void {dodgeable = true; }),
			dodgeTimer,
			false);
	}
}

void APlayerCharacter::InputTurn(float value)
{
	this->AddControllerYawInput(value);
}

void APlayerCharacter::InputLookUp(float value)
{
	if (camYMove) {
		this->AddControllerPitchInput(value);
	}
}

#pragma endregion

#pragma region Attack
void APlayerCharacter::OneHandUse()
{
	AWeapon* weapon = Cast<AWeapon>(currentWeapon);
	if (isTwoHand == true) {
		isTwoHand = false;
		isOneHand = true;
	}
	else
		isOneHand = true;
	weapon->weaponDamage = 20.0f + weapon->damagePlus;;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Damage is : %f"), weapon->weaponDamage));
	UPointLightComponent* onehandLight = weapon->spotLight_OneHand;
	UPointLightComponent* twohandLight = weapon->spotLight_TwoHand;
	onehandLight->SetVisibility(true);
	twohandLight->SetVisibility(false);

}

void APlayerCharacter::TwoHandUse()
{
	if (isOneHand == true) {
		isOneHand = false;
		isTwoHand = true;

	}
	else
		isTwoHand = true;

	AWeapon* weapon = Cast<AWeapon>(currentWeapon);
	weapon->weaponDamage = 50.0f + weapon->damagePlus;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Damage is : %f"), weapon->weaponDamage));
	UPointLightComponent* onehandLight = weapon->spotLight_OneHand;
	UPointLightComponent* twohandLight = weapon->spotLight_TwoHand;
	onehandLight->SetVisibility(false);
	twohandLight->SetVisibility(true);
}

void APlayerCharacter::Attack_OneHand()
{

	if (!isDuringAttack) {
		if (isNone)
			isDuringAttack = false;
		if (isOneHand)
			PlayAnimMontage(OneHand_Attack_Base, 1.0f);
		if (isTwoHand)
			PlayAnimMontage(TwoHand_Attack_Base, 1.0f);
	}
}

void APlayerCharacter::Attack_OneHand_Q()
{
	if (!isDuringAttack && skillQalbe) {
		isDuringAttack = true;
		skillQalbe = false;
		if (isNone)
			isDuringAttack = false;
		if (isOneHand)
			PlayAnimMontage(OneHand_Attack_Q, 1.5f);
		if (isTwoHand)
			PlayAnimMontage(TwoHand_Attack_Q, 1.5f);

		FTimerHandle skillQTimeHandle;
		GetWorldTimerManager().SetTimer(
			skillQTimeHandle,
			FTimerDelegate::CreateLambda([this]()->void {skillQalbe = true; }),
			skillTimer,
			false);
	}
}

void APlayerCharacter::Attack_OneHand_E()
{
	if (!isDuringAttack && skillEable) {
		isDuringAttack = true;
		skillEable = false;
		if (isNone)
			isDuringAttack = false;
		if (isOneHand)
			PlayAnimMontage(OneHand_Attack_E, 1.5f);
		if (isTwoHand)
			PlayAnimMontage(TwoHand_Attack_E, 1.5f);

		FTimerHandle skillETimeHandle;
		GetWorldTimerManager().SetTimer(
			skillETimeHandle,
			FTimerDelegate::CreateLambda([this]()->void {skillEable = true; }),
			skillTimer,
			false);
	}
}

void APlayerCharacter::Attack_OneHand_C()
{
	if (!isDuringAttack && skillCable) {
		isDuringAttack = true;
		skillCable = false;
		if (isNone)
			isDuringAttack = false;
		if (isOneHand)
			PlayAnimMontage(OneHand_Attack_C, 1.5f);
		if (isTwoHand)
			PlayAnimMontage(TwoHand_Attack_C, 1.5f);

		FTimerHandle skillCTimeHandle;
		GetWorldTimerManager().SetTimer(
			skillCTimeHandle,
			FTimerDelegate::CreateLambda([this]()->void {skillCable = true; }),
			skillTimer,
			false);
	}
}

void APlayerCharacter::Attack_OneHand_End()
{
	isDuringAttack = false;
}
void APlayerCharacter::AttackCheck()
{
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

#if ENABLE_DRAW_DEBUG

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

#endif

	if (bResult) {
		if (HitResult.Actor.IsValid() && HitResult.Actor->IsA(AEnemy::StaticClass())) {
			AWeapon* currentweapon = currentWeapon;
			hit_enemy = Cast<AEnemy>(HitResult.Actor);
			if (hit_enemy->enemyName.ToString() != "Boss" && hit_enemy->health - currentweapon->weaponDamage <= 0.0f) {
				hit_enemy->PlayAnimMontage(hit_enemy->Death_AnimMontage, 1.0f);
				hit_enemy->Die();
			}
			else
			{
				hit_enemy->health -= currentweapon->weaponDamage;
			}

			if (hit_enemy->enemyName.ToString() == "Boss") {
				if (hit_enemy->health / hit_enemy->maxHealth <= 0.8f && bossHealth80 == false) {
					bossHealth80 = true;
					hit_enemy->BossFunction80();
					if (tempMontage == 0) {
						hit_enemy->PlayAnimMontage(hit_enemy->start80, 1.0f);
						tempMontage++;
					}
				}
				if (hit_enemy->health / hit_enemy->maxHealth <= 0.5f && bossHealth50 == false) {
					bossHealth50 = true;
					hit_enemy->BossFunction50();
					if (boss50MT == 0) {
						hit_enemy->PlayAnimMontage(hit_enemy->boss50, 1.0f);
						boss50MT++;
					}
				}
				if (hit_enemy->health / hit_enemy->maxHealth <= 0.3f && bossHealth30 == false) {
					bossHealth30 = true;
					hit_enemy->BossFunction30();
				}

				if (hit_enemy->health - currentWeapon->weaponDamage <= 0)
				{
					if (hit_enemy->nextLevelClass != nullptr) {
						FVector vpos = this->GetActorLocation();
						FVector spawnLocation = FVector(vpos.X - 150.0f, vpos.Y, vpos.Z);

						FActorSpawnParameters params;
						params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

						GetWorld()->SpawnActor<AActor>(hit_enemy->nextLevelClass, spawnLocation, GetActorRotation(), params);
						UE_LOG(LogTemp, Warning, TEXT("Spawn nextLevel"));
					}
					hit_enemy->PlayAnimMontage(hit_enemy->Death_AnimMontage, 1.0f);
					hit_enemy->Die();
				}
			}

			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Attack"));
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(TEXT("Damage is : %f"), hit_enemy->health));
		}
	}
	
}

#pragma endregion
