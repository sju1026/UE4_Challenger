/*
* Writer : seo jae ung
*
* Function
*  - 특정 AI 캐릭터가 무작위 위치로 이동하도록 돕는 행동 트리 작업을 정의합니다.
*  - 캐릭터는 현재 위치에서 특정 반경 내에서 무작위 지점을 선택하고, 해당 지점을 목표 위치로 설정하여 이동합니다. 
*  - 코드는 AI 컨트롤러에게 현재 위치에서 무작위 위치를 찾아 설정하도록 지시하는 역할을 합니다. 이렇게 설정된 위치 정보는 행동 트리의 블랙보드(Blackboard)에 저장되어 사용됩니다.
* 
* UpdateRate : 2023 - 11 - 29
*/


#include "BTTask_RandomMove.h"
#include <Portfolio_3.h>
#include <BehaviorTree/BTNode.h>
#include <BehaviorTree/BehaviorTreeComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/Blackboard/BlackboardKeyAllTypes.h>
#include <NavigationSystem.h>
#include <EnemyAIController.h>
#include <Engine/World.h>
#include <blackboard_keys.h>

UBTTask_RandomMove::UBTTask_RandomMove(FObjectInitializer const& object_initializer) {
	NodeName = TEXT("Find Random Location");
}

EBTNodeResult::Type UBTTask_RandomMove::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	auto const cont = Cast<AEnemyAIController>(owner_comp.GetAIOwner());
	auto const npc = cont->GetPawn();

	FVector const origin = npc->GetActorLocation();
	FNavLocation loc;

	UNavigationSystemV1* const nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());

	if (nav_sys->GetRandomPointInNavigableRadius(origin, search_radius, loc, nullptr))
	{
		cont->get_blackboard()->SetValueAsVector(bb_keys::target_location, loc.Location);
	}

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
