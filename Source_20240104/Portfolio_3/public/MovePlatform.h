// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovePlatform.generated.h"

UCLASS()
class PORTFOLIO_3_API AMovePlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovePlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Base)
		class UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Base)
		class UBoxComponent* collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Base)
		bool isMove;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Base)
		bool max;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Base)
		float moveValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Base)
		float maxZPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Base)
		float minZPos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Base)
		float plusValue;

private:
	UFUNCTION()
		void EnterPlayer(class AActor* overlappedActor, class AActor* otherActor);

	UFUNCTION()
		void ExitPlayer(class AActor* overlappedActor, class AActor* otherActor);

	FTimerHandle* actorTimeHandler;
};
