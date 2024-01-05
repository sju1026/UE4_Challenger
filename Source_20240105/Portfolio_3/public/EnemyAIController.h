// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include <Perception/AIPerceptionTypes.h>
#include "EnemyAIController.generated.h"

/**
 * 
 */

class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS(config = Game)
class PORTFOLIO_3_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public :
	AEnemyAIController(FObjectInitializer const& object_initializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* pawn) override;
	virtual void Tick(float DeltaSeconds) override;

	virtual FRotator GetControlRotation() const override;

	UFUNCTION(BlueprintCallable, Category = Behavior)
		void on_target_detected(AActor* actor, FAIStimulus const stimulus);

	class UBlackboardComponent* get_blackboard() const;

private:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent* behavior_tree_comp;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree* btree;

	class UBlackboardComponent* blackboard;
	class UAISenseConfig_Sight* sight_config;

	void setup_perception_system();
};
