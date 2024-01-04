// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include <GameFramework/Actor.h>
#include <PlayerCharacter.h>
#include <Enemy.h>
#include <GameFramework/SpringArmComponent.h>

#include "Teleport.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_3_API ATeleport : public ATriggerBox
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

public:
	ATeleport();

	UFUNCTION()
		void EnterTeleporter(class AActor* overlappedActor, class AActor* otherActor);

	UFUNCTION()
		void ExitTeleporter(class AActor* overlappedActor, class AActor* otherActor);

	UPROPERTY(EditAnywhere, Category = Teleporter)
		ATeleport* otherTele;

	UPROPERTY()
		bool teleporting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
		TArray<FString> elements;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = State)
		int elementNum;



};
