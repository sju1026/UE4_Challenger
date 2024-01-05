// Copyright Epic Games, Inc. All Rights Reserved.


#include "Portfolio_3GameModeBase.h"
#include <PlayerCharacter.h>
#include <HUD_InGame.h>
#include <UObject/ConstructorHelpers.h>
#include <Teleport.h>
#include <GameFramework/Actor.h>
#include <Kismet/GameplayStatics.h>

APortfolio_3GameModeBase::APortfolio_3GameModeBase() {
	static ConstructorHelpers::FClassFinder<APlayerCharacter>player_class(TEXT("Blueprint'/Game/_My/Blueprints/BP_Player.BP_Player_C'"));
	if (player_class.Succeeded()) {
		DefaultPawnClass = player_class.Class;
	}

	HUDClass = AHUD_InGame::StaticClass();



}

void APortfolio_3GameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

