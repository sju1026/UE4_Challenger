/*
* Writer : seo jae ung
*
* Function
*  - 적 캐릭터의 AI가 플레이어를 공격하는 Behavior Tree Task를 구현합니다. 
*  - 캐릭터가 몽타주 애니메이션을 실행하고, 애니메이션이 종료되면 공격 동작을 수행합니다.
*
* UpdateRate : 2024 - 02 - 13
*/

#include "BTTask_Attack.h"
#include <BehaviorTree/BlackboardComponent.h>
#include <Kismet/GameplayStatics.h>
#include <PlayerCharacter.h>
#include <EnemyAIController.h>
#include <Enemy.h>
#include <blackboard_keys.h>

UBTTask_Attack::UBTTask_Attack(FObjectInitializer const& object_initializer) {
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	Super::ExecuteTask(owner_comp, node_memory);

	if (owner_comp.GetAIOwner() == nullptr) return EBTNodeResult::Failed;

	AEnemy* enemy = Cast<AEnemy>(owner_comp.GetAIOwner()->GetPawn());
	APlayerCharacter* player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));

	if(enemy == nullptr) return EBTNodeResult::Failed;

	if (FVector::Dist(enemy->GetActorLocation(), player->GetActorLocation()) <= attackRange) {
		enemy->EnemyAttack();
		UE_LOG(LogTemp, Warning, TEXT("Player Attack_Enemy"));
	}

	return EBTNodeResult::Succeeded;
}

