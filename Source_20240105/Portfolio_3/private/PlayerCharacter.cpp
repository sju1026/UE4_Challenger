/*
* Writer : seo jae ung
*
* Function
* - 플레이어 캐릭터 클래스로, 캐릭터의 이동, 점프, 대시, 카메라 회전, 공격 및 상호작용과 같은 기능을 구현합니다. 
* - 또한 무기 사용 및 전환, 애니메이션 실행과 관련된 기능을 포함합니다.
* - PrimaryActorTick을 통해 주기적인 Tick을 활성화하며, 입력에 따라 캐릭터의 행동을 제어합니다.
* - 상호작용을 통해 상자를 열거나 무기를 전환하며, 각종 동작을 애니메이션으로 구현하고 있습니다.
*
* UpdateRate : 2023 - 12 - 10
*/

#include "PlayerCharacter.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/PlayerController.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <UObject/ConstructorHelpers.h>
#include <Weapon.h>
#include <ItemBox.h>

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
	dodgeDistance = 4000.0f;
	isdodge = false;
	sprintSpeed = 300.0f;

	isForest = false;
	isStone = false;
	isWater = false;
}

void APlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SpawnDefaultInventory();
}

void APlayerCharacter::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	myHPbar_Text = FString::SanitizeFloat(health) + "/" + FString::SanitizeFloat(maxHealth);
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
	playerInput->BindAction("Interaction", IE_Pressed, this, &APlayerCharacter::Interaction);
	playerInput->BindAction("Interaction", IE_Released, this, &APlayerCharacter::Interaction_End);
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
	isdodge = true;
	if (isdodge == true) {
		const FVector forwardDir = this->GetActorRotation().Vector();
		LaunchCharacter(forwardDir * dodgeDistance, true, true);
	}
	GetWorld()->GetTimerManager().SetTimer(playerTime, FTimerDelegate::CreateLambda([this]()->void {isdodge = false; }), 2.0f, false);
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
void APlayerCharacter::Interaction()
{
	isBoxOpen = true;
}
void APlayerCharacter::Interaction_End()
{
	isBoxOpen = false;
}
#pragma endregion

#pragma region Attack
void APlayerCharacter::OneHandUse()
{
	if (isTwoHand == true) {
		isTwoHand = false;
		isOneHand = true;
	}
	else
		isOneHand = true;
	AWeapon* weapon = Cast<AWeapon>(currentWeapon);
	weapon->weaponDamage = 20.0f + weapon->damagePlus;;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Damage is : %f"), weapon->weaponDamage));
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
	if (!isDuringAttack) {
		isDuringAttack = true;
		if (isNone)
			isDuringAttack = false;
		if (isOneHand)
			PlayAnimMontage(OneHand_Attack_Q, 1.5f);
		if (isTwoHand)
			PlayAnimMontage(TwoHand_Attack_Q, 1.5f);
	}
}

void APlayerCharacter::Attack_OneHand_E()
{
	if (!isDuringAttack) {
		isDuringAttack = true;
		if (isNone)
			isDuringAttack = false;
		if (isOneHand)
			PlayAnimMontage(OneHand_Attack_E, 1.5f);
		if (isTwoHand)
			PlayAnimMontage(TwoHand_Attack_E, 1.5f);
	}
}

void APlayerCharacter::Attack_OneHand_C()
{
	if (!isDuringAttack) {
		isDuringAttack = true;
		if (isNone)
			isDuringAttack = false;
		if (isOneHand)
			PlayAnimMontage(OneHand_Attack_C, 1.5f);
		if (isTwoHand)
			PlayAnimMontage(TwoHand_Attack_C, 1.5f);
	}
}

void APlayerCharacter::Attack_OneHand_End()
{
	isDuringAttack = false;
}
#pragma endregion
