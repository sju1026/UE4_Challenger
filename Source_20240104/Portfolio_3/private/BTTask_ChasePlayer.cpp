/*
* Writer : seo jae ung
*
* Function
*  - AI 캐릭터가 플레이어를 추적하고 이동하는 Behavior Tree Task입니다. 
*  - AI 컨트롤러는 블랙보드에서 플레이어의 위치를 가져와서 해당 위치로 이동하는 작업을 수행합니다.
*  - 코드는 AI 컨트롤러를 통해 블랙보드에 플레이어의 위치 정보를 저장하여 사용하게 됩니다.
*
* UpdateRate : 2023 - 11 - 29
*/


#include "BTTask_ChasePlayer.h"
#include <EnemyAIController.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Blueprint/AIBlueprintHelperLibrary.h>
#include <blackboard_keys.h>

UBTTask_ChasePlayer::UBTTask_ChasePlayer(FObjectInitializer const& object_initializer) {
	NodeName = TEXT("Player Chase");
}

EBTNodeResult::Type UBTTask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	AEnemyAIController* const cont = Cast<AEnemyAIController>(owner_comp.GetAIOwner());
	FVector const player_location = cont->get_blackboard()->GetValueAsVector(bb_keys::target_location);

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, player_location);


	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
