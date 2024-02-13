// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeadZone.generated.h"

UCLASS()
class PORTFOLIO_3_API ADeadZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeadZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Base)
		UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Base)
		class UBoxComponent* trigger;

private:
	UFUNCTION()
		void EnterPlayer(class AActor* overlappedActor, class AActor* otherActor);

	UFUNCTION()
		void TimerFunction();

	FTimerHandle actorTimeHandler;

	int repeatingCallsRemaining;

	class APlayerCharacter* player;
};
