/*
* Writer : seo jae ung
*
* Function
*  - 아이템 클래스는 플레이어와 상호작용하여 플레이어의 체력을 회복시키는 아이템입니다. 
*  - 플레이어와의 충돌을 감지하고, 플레이어가 최대 체력을 초과하지 않도록 체력을 증가시킵니다.
*  - 이후 아이템은 소멸합니다.
* UpdateRate : 2024 - 01 - 05
*/

#include "Item.h"
#include <PlayerCharacter.h>
#include <Weapon.h>

// Sets default values
AItem::AItem()
{
	OnActorBeginOverlap.AddDynamic(this, &AItem::EnterPlayer);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	trigger = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	sphereMesh = CreateDefaultSubobject<UStaticMeshComponent>("BoxMesh");

	RootComponent = trigger;
	sphereMesh->SetupAttachment(RootComponent);

	trigger->SetBoxExtent(FVector(70.0f, 72.0f, 50.0f));
	trigger->SetRelativeLocation(FVector(0, 0, 20.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Item_Box(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));
	if (Item_Box.Succeeded())
	{
		sphereMesh->SetStaticMesh(Item_Box.Object);
	}
	sphereMesh->SetRelativeLocation(FVector(0.0f, -3.5f, -53.0f));

	sphereMesh->SetCollisionProfileName(TEXT("NoCollision"));

	value = 50;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItem::EnterPlayer(AActor* overlappedActor, AActor* otherActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Collision!");
	if (otherActor->IsA(APlayerCharacter::StaticClass())) {
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Collision2");
		APlayerCharacter* player = Cast<APlayerCharacter>(otherActor);
		if (itemType.ToString() == "HealthPotion") {
			if (player->health + value >= player->maxHealth) {
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("OverHealth"));
				player->health = player->maxHealth;
			}
			else {
				player->health += value;
			}
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("HP is : %f"), player->health));
		}
		if (itemType.ToString() == "DamageItem") {
			AWeapon* weapon = Cast<AWeapon>(player->currentWeapon);
			weapon->damagePlus += value;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Damage is : %f"), weapon->weaponDamage));
		}
		if (itemType.ToString() == "SpeedItem") {
			player->walkSpeed += value;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Speed is : %f"), player->walkSpeed));
		}
		if (itemType.ToString() == "MaxHealthItem") {
			player->maxHealth += value;
			player->health += value;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("MAXHP is : %f"), player->maxHealth));
		}
		Destroy();
	}
}

