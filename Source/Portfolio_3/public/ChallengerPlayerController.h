// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChallengerPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_3_API AChallengerPlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UUserWidget> gameUIClass;
	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<class UUserWidget> gameoverUIClass;

	UPROPERTY()
		UUserWidget* hud;
};
