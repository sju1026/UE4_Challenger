// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTransferVolume.generated.h"

UCLASS()
class PORTFOLIO_3_API ALevelTransferVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelTransferVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void NotifyActorBeginOverlap(AActor* otherActor) override;

private:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FName transferLevelName;

	UPROPERTY()
		class UBoxComponent* transferVolume;

};
