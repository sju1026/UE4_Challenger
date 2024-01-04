/*
* Writer : seo jae ung
*
* Function
* - 
*
* UpdateRate : 2024 - 01 - 03
*/

#include "DeadZone.h"
#include <Components/BoxComponent.h>
#include <PlayerCharacter.h>
#include "Engine/World.h"

// Sets default values
ADeadZone::ADeadZone()
{
	OnActorBeginOverlap.AddDynamic(this, &ADeadZone::EnterPlayer);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));

	RootComponent = trigger;
	mesh->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>zone_Box(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'"));
	if (zone_Box.Succeeded())
	{
		mesh->SetStaticMesh(zone_Box.Object);
	}

	trigger->SetBoxExtent(FVector(80000.0f, 80000.0f, 60.0f));
	trigger->SetRelativeLocation(FVector(0, 0, 100.0f));

	mesh->SetWorldScale3D(FVector(1500.0f, 1500.0f, 0.3f));
	mesh->SetRelativeLocation(FVector(0, 0, -15.0f));

	repeatingCallsRemaining = 2;

}

// Called when the game starts or when spawned
void ADeadZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADeadZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADeadZone::EnterPlayer(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor->IsA(APlayerCharacter::StaticClass())) {
		player = Cast<APlayerCharacter>(otherActor);
		player->health = 0.0f;
		player->PlayAnimMontage(player->Death_AnimMontage, 1.0f);

		GetWorld()->GetTimerManager().SetTimer(actorTimeHandler, this, &ADeadZone::TimerFunction, 1.0f, true, 1.0f);
	}
}

void ADeadZone::TimerFunction()
{
	if (--repeatingCallsRemaining <= 0) {
		if (player) {
			APlayerController* con = Cast<APlayerController>(player->Controller);
			con->SetPause(true);
			GetWorldTimerManager().ClearTimer(actorTimeHandler);
		}
	}
}

