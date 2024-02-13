// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include <Enemy.h>
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_3_API UBTTask_Attack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_Attack(FObjectInitializer const& object_initializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)override;

private:
	float attackRange = 250.0f;
};
