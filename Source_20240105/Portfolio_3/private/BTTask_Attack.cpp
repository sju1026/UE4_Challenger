/*
* Writer : seo jae ung
*
* Function
*  - 적 캐릭터의 AI가 플레이어를 공격하는 Behavior Tree Task를 구현합니다. 
*  - 캐릭터가 몽타주 애니메이션을 실행하고, 애니메이션이 종료되면 공격 동작을 수행합니다.
*
* UpdateRate : 2023 - 11 - 29
*/

#include "BTTask_Attack.h"
#include <EnemyAIController.h>
#include <Kismet/KismetSystemLibrary.h>
#include <Engine/World.h>
#include <Engine/LatentActionManager.h>
#include <Components/SkeletalMeshComponent.h>
#include <Animation/AnimMontage.h>
#include <Animation/AnimInstance.h>
#include <AIController.h>
#include <BaseCharacter.h>
#include <Enemy.h>
#include <CombatInterface.h>

UBTTask_Attack::UBTTask_Attack(FObjectInitializer const& object_initializer) {
	NodeName = TEXT("Enemy Attack");
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	AAIController* const cont = owner_comp.GetAIOwner();
	AEnemy* const npcBot = Cast<AEnemy>(cont->GetPawn());
	if (ICombatInterface* const icombat = Cast<ICombatInterface>(npcBot))
	{
		if (montage_has_finished(npcBot)) {
			icombat->Execute_melee_attack(npcBot);
		}
	}
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

bool UBTTask_Attack::montage_has_finished(AEnemy* const npcBot)
{
	return npcBot->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(npcBot->get_montage());
}
