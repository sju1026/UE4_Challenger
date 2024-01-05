// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include <Components/StaticMeshComponent.h>
#include <Weapon.h>
#include <Item.h>
#include "ItemBox.generated.h"

UCLASS()
class PORTFOLIO_3_API AItemBox : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemBox();

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
		UStaticMeshComponent* boxMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Box)
		TArray<TSubclassOf<AItem>> ItemClasses;

private:
	UFUNCTION()
		void EnterTeleporter(class AActor* overlappedActor, class AActor* otherActor);
};
