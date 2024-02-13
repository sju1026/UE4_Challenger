/*
* Writer : seo jae ung
*
* Function
*  - 적 캐릭터의 공격 범위 내에 플레이어가 있는지 확인하는 Behavior Tree 서비스입니다.
*  - 서비스는 플레이어와 적 캐릭터 사이의 거리를 계산하고, 해당 정보를 블랙보드에 업데이트합니다.
*
* UpdateRate : 2024 - 02 - 13
*/

#include "BTService_IsPlayerInAttackRange.h"
#include "Enemy.h"
#include <EnemyAIController.h>
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <blackboard_keys.h>
#include <Kismet/GameplayStatics.h>

UBTService_IsPlayerInAttackRange::UBTService_IsPlayerInAttackRange() {
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Is Player in Attack Range");
}

void UBTService_IsPlayerInAttackRange::OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_momory)
{
	Super::OnBecomeRelevant(owner_comp, node_momory);

	AEnemyAIController* const cont = Cast<AEnemyAIController>(owner_comp.GetAIOwner());
	AEnemy* const npcBot = Cast<AEnemy>(cont->GetPawn());

	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	cont->get_blackboard()->SetValueAsBool(GetSelectedBlackboardKey(), npcBot->GetDistanceTo(player) <= Attack_range);
}
