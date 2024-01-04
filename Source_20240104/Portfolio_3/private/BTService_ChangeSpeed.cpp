/*
* Writer : seo jae ung
*
* Function
*  - 적 캐릭터의 이동 속도를 변경하는 Behavior Tree 서비스입니다. 
*  - 서비스는 캐릭터의 최대 이동 속도를 설정하고, 해당 정보를 업데이트합니다. "Change Speed"라는 이름으로 적 캐릭터의 속도를 조정합니다.
*
* UpdateRate : 2023 - 11 - 29
*/

#include "BTService_ChangeSpeed.h"
#include <Enemy.h>
#include <EnemyAIController.h>
#include <GameFramework/Character.h>
#include <GameFramework/CharacterMovementComponent.h>

UBTService_ChangeSpeed::UBTService_ChangeSpeed() {
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change Speed");
}

void UBTService_ChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_momory)
{
	Super::OnBecomeRelevant(owner_comp, node_momory);
	auto const cont = owner_comp.GetAIOwner();
	AEnemy* const npcBot = Cast<AEnemy>(cont->GetPawn());
	npcBot->GetCharacterMovement()->MaxWalkSpeed = speed;
}

FString UBTService_ChangeSpeed::GetStaticServiceDescription() const
{
	return FString("Change the Enmey Speed");
}
