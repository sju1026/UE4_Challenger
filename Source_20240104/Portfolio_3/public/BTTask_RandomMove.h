// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RandomMove.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PORTFOLIO_3_API UBTTask_RandomMove : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_RandomMove(FObjectInitializer const& object_initializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = "true"))
		float search_radius = 1500.0f;
};
