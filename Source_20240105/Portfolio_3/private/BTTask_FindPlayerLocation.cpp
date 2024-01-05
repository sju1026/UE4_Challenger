/*
* Writer : seo jae ung
*
* Function
*  - 적 AI가 플레이어를 찾아 그 위치를 지정하는 행동 트리 작업을 담고 있습니다. 
*  - 주요 기능으로는 플레이어 캐릭터의 위치를 확인하고, 무작위 위치를 찾거나 플레이어의 위치를 목표로 설정하여 AI 캐릭터가 해당 위치로 이동하도록 지시합니다. 
*  - 코드는 AI 컨트롤러를 통해 블랙보드에 플레이어의 위치 정보를 저장하여 사용하게 됩니다.
*
* UpdateRate : 2023 - 11 - 29
*/

#include "BTTask_FindPlayerLocation.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <NavigationSystem.h>
#include <EnemyAIController.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Vector.h>
#include <Kismet/GameplayStatics.h>
#include <GameFramework/Character.h>
#include <blackboard_keys.h>

UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& object_initializer) {
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	ACharacter* const palyer = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto const cont = Cast<AEnemyAIController>(owner_comp.GetAIOwner());

	FVector player_location = palyer->GetActorLocation();

	if (search_random) {
		FNavLocation loc;

		UNavigationSystemV1* const nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
		if (nav_sys->GetRandomPointInNavigableRadius(player_location, search_radius, loc, nullptr)) {
			cont->get_blackboard()->SetValueAsVector(bb_keys::target_location, loc.Location);
		}
	}
	else {
		cont->get_blackboard()->SetValueAsVector(bb_keys::target_location, player_location);
	}

	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Type();
}
