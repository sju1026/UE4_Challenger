/*
* Writer : seo jae ung
*
* Function
* - 플레이어 캐릭터가 상자와 상호 작용할 때 아이템을 생성합니다. 
* - 플레이어 캐릭터가 '트리거(Trigger)' 박스 컴포넌트와 겹칠 때, 플레이어의 'isBoxOpen' 플래그를 확인하여 참이면 특정 위치에 아이템을 생성합니다. 
* - 아이템은 상자 위치를 기준으로 일정한 오프셋(FVector(150.0f, 0.0f, 0.0f))을 더한 위치에 생성됩니다. 아이템을 생성한 후에는 아이템 상자를 파괴합니다.
*
* UpdateRate : 2023 - 12 - 10
*/

#include "ItemBox.h"
#include "GameFramework/Actor.h"
#include <PlayerCharacter.h>
#include "UObject/ConstructorHelpers.h"
#include "Math/RandomStream.h"

// Sets default values
AItemBox::AItemBox()
{
	OnActorBeginOverlap.AddDynamic(this, &AItemBox::EnterTeleporter);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	trigger = CreateDefaultSubobject<UBoxComponent>("TriggerBox");
	boxMesh = CreateDefaultSubobject<UStaticMeshComponent>("BoxMesh");

	RootComponent = trigger;
	boxMesh->SetupAttachment(RootComponent);

	trigger->SetBoxExtent(FVector(150.0f, 150.0f, 100.0f));
	trigger->SetRelativeLocation(FVector(0, 0, 20.0f));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>Item_Box(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_WideCapsule.Shape_WideCapsule'"));
	if (Item_Box.Succeeded())
	{
		boxMesh->SetStaticMesh(Item_Box.Object);
	}
	boxMesh->SetRelativeLocation(FVector(0.0f, -3.5f, -53.0f));

	boxMesh->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FClassFinder<AItem>potion_class(TEXT("Blueprint'/Game/_My/Blueprints/Items/BP_Item_HP_potion.BP_Item_HP_Potion_C'"));
	if (potion_class.Succeeded()) {
		ItemClasses.Add(potion_class.Class);
	}
	static ConstructorHelpers::FClassFinder<AItem>damage_class(TEXT("Blueprint'/Game/_My/Blueprints/Items/BP_Item_Damage.BP_Item_Damage_C'"));
	if (damage_class.Succeeded()) {
		ItemClasses.Add(damage_class.Class);
	}
	static ConstructorHelpers::FClassFinder<AItem>speed_class(TEXT("Blueprint'/Game/_My/Blueprints/Items/BP_Item_Speed.BP_Item_Speed_C'"));
	if (speed_class.Succeeded()) {
		ItemClasses.Add(speed_class.Class);
	}
}

// Called when the game starts or when spawned
void AItemBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBox::EnterTeleporter(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor->IsA(APlayerCharacter::StaticClass())) {
		APlayerCharacter* player = Cast<APlayerCharacter>(otherActor);
		if (player->isBoxOpen == true) {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Collision!");

			FVector vpos = this->GetActorLocation();
			FVector spawnLocation = FVector(vpos.X + 150.0f, vpos.Y, vpos.Z);

			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

			FDateTime Now = FDateTime::Now();
			int32 Seed = Now.GetSecond() + Now.GetMillisecond();
			FRandomStream RandomStream(Seed);
			int32 RandomNumber = RandomStream.RandRange(0, 2);

			GetWorld()->SpawnActor<AItem>(ItemClasses[RandomNumber], spawnLocation, GetActorRotation(), params);

			Destroy();
		}
	}
}
