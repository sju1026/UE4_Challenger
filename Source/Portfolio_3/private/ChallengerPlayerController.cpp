// Fill out your copyright notice in the Description page of Project Settings.


#include "ChallengerPlayerController.h"
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include <PlayerCharacter.h>

void AChallengerPlayerController::BeginPlay() {
	Super::BeginPlay();

	hud = CreateWidget(this, gameUIClass);

	if (hud != nullptr) {
		hud->AddToViewport();
	}
}

void AChallengerPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (player != nullptr) {
		if (player->health <= 0) {
			hud->RemoveFromViewport();
			UUserWidget* gameover = CreateWidget(this, gameoverUIClass);
			if (gameoverUIClass != nullptr) {
				bShowMouseCursor = true;
				gameover->AddToViewport();
			}
		}
	}
}