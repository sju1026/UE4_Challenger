/*
* Writer : seo jae ung
*
* Function
* - 플레이어 캐릭터의 애니메이션 상태 변경을 감지하는 클래스입니다. 
* - NotifyBegin과 NotifyEnd 함수는 애니메이션 상태의 시작과 끝을 감지하고, 해당 상태에 따라 플레이어 캐릭터의 공격 동작을 제어합니다. 
* - 이 클래스는 애니메이션 이벤트에 연결되어 공격 중일 때 상태를 설정하고, 공격이 끝나면 해당 동작을 종료합니다.
*
* UpdateRate : 2024 - 02 - 13
*/


#include "PlayerAnimNotifyState.h"
#include "PlayerCharacter.h"

void UPlayerAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, __FUNCTION__);

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL) {
		APlayerCharacter* player = Cast<APlayerCharacter>(MeshComp->GetOwner());
		if (player != NULL) {
			player->isDuringAttack = true;
			player->AttackCheck();
		}
	}
}

void UPlayerAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) {
	// GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, __FUNCTION__);

	if (MeshComp != NULL && MeshComp->GetOwner() != NULL) {
		APlayerCharacter* player = Cast<APlayerCharacter>(MeshComp->GetOwner());
		if (player != NULL) {
			player->Attack_OneHand_End();
		}
	}
}