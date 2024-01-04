/*
* Writer : seo jae ung
*
* Function
* - 플레이어 캐릭터가 플랫폼에 진입하거나 떠날 때 플랫폼을 움직이는 클래스입니다. 
* - EnterPlayer와 ExitPlayer 함수는 플레이어가 플랫폼에 진입하거나 떠날 때의 상호작용을 처리하며, Tick 함수에서는 플랫폼의 움직임을 제어합니다. 
* - 플랫폼은 플레이어의 진입 여부에 따라 위아래로 움직이게 됩니다.
*
* UpdateRate : 2023 - 12 - 10
*/

#include "MovePlatform.h"
#include <Components/BoxComponent.h>
#include <PlayerCharacter.h>

// Sets default values
AMovePlatform::AMovePlatform()
{
	OnActorBeginOverlap.AddDynamic(this, &AMovePlatform::EnterPlayer);
	OnActorEndOverlap.AddDynamic(this, &AMovePlatform::ExitPlayer);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));

	RootComponent = collision;
	mesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>mesh_obj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (mesh_obj.Succeeded()) {
		mesh->SetStaticMesh(mesh_obj.Object);
	}

	collision->SetBoxExtent(FVector(200.0f, 200.0f, 60.0f));
	collision->SetRelativeLocation(FVector(0, 0, 100.0f));

	mesh->SetWorldScale3D(FVector(3.0f, 3.0f, 0.3f));

	isMove = false;
	max = false;
	moveValue = 1.2f;
}

// Called when the game starts or when spawned
void AMovePlatform::BeginPlay()
{
	Super::BeginPlay();

	FVector location = this->GetActorLocation();
	maxZPos = location.Z + plusValue;
	minZPos = location.Z;
}

// Called every frame
void AMovePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector location = this->GetActorLocation();
	if (isMove == true) {
		if (max == true) {
			location.Z -= moveValue;
			SetActorLocation(FVector(location));
			if (location.Z <= minZPos) {
				max = false;
			}
		}
		else if (max == false) {
			location.Z += moveValue;
			SetActorLocation(FVector(location));
			if (location.Z >= maxZPos) {
				max = true;
			}
		}
	}
}

void AMovePlatform::EnterPlayer(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor->IsA(APlayerCharacter::StaticClass())) {
		isMove = true;
	}
}

void AMovePlatform::ExitPlayer(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor->IsA(APlayerCharacter::StaticClass())) {
		FVector location = this->GetActorLocation();
		SetActorLocation(FVector(location.X, location.Y, minZPos));
		isMove = false;
	}
}

