// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransferVolume.h"
#include <Components/BoxComponent.h>
#include <Kismet/GameplayStatics.h>
#include <PlayerCharacter.h>

// Sets default values
ALevelTransferVolume::ALevelTransferVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	transferVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger"));
	RootComponent = transferVolume;
	transferVolume->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
}

// Called when the game starts or when spawned
void ALevelTransferVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelTransferVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelTransferVolume::NotifyActorBeginOverlap(AActor* otherActor)
{
	if (otherActor->IsA(APlayerCharacter::StaticClass())) {
		UGameplayStatics::OpenLevel(this, transferLevelName);
	}
}

