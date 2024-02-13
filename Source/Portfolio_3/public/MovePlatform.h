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

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Base, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Base, meta = (AllowPrivateAccess = "true"))
		class UBoxComponent* collision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light, meta = (AllowPrivateAccess = "true"))
		class USpotLightComponent* spotLight_In;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Light, meta = (AllowPrivateAccess = "true"))
		class USpotLightComponent* spotLight_Out;

	UFUNCTION()
		void EnterPlayer(class AActor* overlappedActor, class AActor* otherActor);

	UFUNCTION()
		void ExitPlayer(class AActor* overlappedActor, class AActor* otherActor);

#pragma region Moving

	UPROPERTY(EditAnywhere, Category = "Moving")
		FVector platformVelocity = FVector(100.0f, 0, 0);
	UPROPERTY(EditAnywhere, Category = "Moving")
		float moveDistance = 100.0f;
	UPROPERTY(EditAnywhere, Category = "Rotation")
		FRotator rotationVelocity;

	FVector startLocation;

	void MovePlatform(float deltaTime);

	void RotatePlatform(float deltaTime);

	bool ShouldPlatformReturn() const;
	float GetDistanceMoved() const;

#pragma endregion

};
