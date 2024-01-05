/*
* Writer : seo jae ung
*
* Function
* - 플레이어 캐릭터의 애니메이션을 업데이트하는 클래스입니다. 
* - NativeUpdateAnimation 함수 내에서 플레이어 캐릭터의 움직임 상태와 현재 무기 상태를 확인하여 해당 정보를 수집합니다. 
* - 이를 통해 애니메이션에 필요한 상태를 업데이트하고 반영합니다.
*
* UpdateRate : 2023 - 11 - 30
*/

#include "PlayerAnim.h"
#include "PlayerCharacter.h"
#include <GameFramework/CharacterMovementComponent.h>

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto ownerPawn = TryGetPawnOwner();

	auto player = Cast<APlayerCharacter>(ownerPawn);

	if (player) {
		auto movement = player->GetCharacterMovement();
		isInAir = movement->IsFalling();

		isOneHand = player->isOneHand;
		isTwoHand = player->isTwoHand;
	}
}