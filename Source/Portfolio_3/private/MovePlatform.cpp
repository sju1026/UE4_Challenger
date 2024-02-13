/*
* Writer : seo jae ung
*
* Function
* - 플레이어 캐릭터가 플랫폼에 진입하거나 떠날 때 플랫폼을 움직이는 클래스입니다. 
* - EnterPlayer와 ExitPlayer 함수는 플레이어가 플랫폼에 진입하거나 떠날 때의 상호작용을 처리하며, Tick 함수에서는 플랫폼의 움직임을 제어합니다. 
* - 플랫폼은 플레이어의 진입 여부에 따라 위아래로 움직이게 됩니다.
*
* UpdateRate : 2024 - 02 - 13
*/

#include "MovePlatform.h"
#include <Components/BoxComponent.h>
#include <PlayerCharacter.h>
#include <Components/SpotLightComponent.h>

// Sets default values
AMovePlatform::AMovePlatform()
{
	OnActorBeginOverlap.AddDynamic(this, &AMovePlatform::EnterPlayer);
	OnActorEndOverlap.AddDynamic(this, &AMovePlatform::ExitPlayer);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	spotLight_In = CreateDefaultSubobject<USpotLightComponent>(TEXT("In_SpotLight"));
	spotLight_Out = CreateDefaultSubobject<USpotLightComponent>(TEXT("Out_SpotLight"));

	RootComponent = collision;
	mesh->SetupAttachment(RootComponent);
	spotLight_In->SetupAttachment(RootComponent);
	spotLight_Out->SetupAttachment(RootComponent);

	spotLight_In->SetRelativeLocation(FVector(0, 0, 277.0f));
	spotLight_In->SetRelativeRotation(FRotator(-90.0f, 0, 0));
	spotLight_In->Intensity = 10000.0f;

	spotLight_Out->SetRelativeLocation(FVector(0, 0, 277.0f));
	spotLight_Out->SetRelativeRotation(FRotator(-90.0f, 0, 0));
	spotLight_Out->Intensity = 10000.0f;

	spotLight_In->SetVisibility(false);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>mesh_obj(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (mesh_obj.Succeeded()) {
		mesh->SetStaticMesh(mesh_obj.Object);
	}

	collision->SetBoxExtent(FVector(200.0f, 200.0f, 60.0f));
	collision->SetRelativeLocation(FVector(0, 0, 100.0f));

	mesh->SetWorldScale3D(FVector(3.0f, 3.0f, 0.3f));

}

// Called when the game starts or when spawned
void AMovePlatform::BeginPlay()
{
	Super::BeginPlay();

	startLocation = GetActorLocation();
}

// Called every frame
void AMovePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlatform(DeltaTime);
	RotatePlatform(DeltaTime);
}

#pragma region Moving

void AMovePlatform::MovePlatform(float deltaTime)
{
	// Reverse direction of motion if gone too far
	if (ShouldPlatformReturn()) {
		FVector moveDirection = platformVelocity.GetSafeNormal();
		startLocation = startLocation + moveDirection * moveDistance;
		SetActorLocation(startLocation);
		platformVelocity = -platformVelocity;
	}
	else
	{
		// Move platform forwards
		FVector currentLocation = GetActorLocation();
		currentLocation += platformVelocity * deltaTime;
		SetActorLocation(currentLocation);
	}
}

void AMovePlatform::RotatePlatform(float deltaTime)
{
	AddActorLocalRotation(rotationVelocity * deltaTime);
}

bool AMovePlatform::ShouldPlatformReturn() const
{
	return GetDistanceMoved() > moveDistance;
}

float AMovePlatform::GetDistanceMoved() const
{
	return FVector::Dist(startLocation, GetActorLocation());
}

#pragma endregion


void AMovePlatform::EnterPlayer(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor->IsA(APlayerCharacter::StaticClass())) {
		spotLight_In->SetVisibility(true);
		spotLight_Out->SetVisibility(false);
	}
}

void AMovePlatform::ExitPlayer(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor->IsA(APlayerCharacter::StaticClass())) {
		spotLight_In->SetVisibility(false);
		spotLight_Out->SetVisibility(true);
	}
}



