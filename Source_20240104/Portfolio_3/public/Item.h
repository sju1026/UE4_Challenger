// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include "Item.generated.h"

UCLASS()
class PORTFOLIO_3_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, Category = Box)
		UBoxComponent* trigger;

	UPROPERTY(VisibleAnywhere, Category = Box)
		UStaticMeshComponent* sphereMesh;

	UPROPERTY(EditAnywhere, Category = State)
		int16 value;

	UPROPERTY(EditAnywhere, Category = ItemName)
		FName itemType;

private:
	UFUNCTION()
		void EnterPlayer(class AActor* overlappedActor, class AActor* otherActor);
};
