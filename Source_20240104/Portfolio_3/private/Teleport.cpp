/*
* Writer : seo jae ung
*
* Function
* - 플레이어 캐릭터를 이동시키는 텔레포트 기능을 구현합니다. 
* - 오버랩이 발생하면 플레이어를 다른 텔레포트 위치로 이동시키고, 이동이 완료되면 해당 상태를 변경합니다. 
* - 이동 중 다른 텔레포트와 충돌 방지를 위해 텔레포트 상태를 관리하고, 출구에서는 이동이 완료되었음을 알려줍니다.
*
* UpdateRate : 2023 - 12 - 10
*/


#include "Teleport.h"
#include <PlayerCharacter.h>
#include <Engine/Classes/Camera/CameraComponent.h>

ATeleport::ATeleport() {
	OnActorBeginOverlap.AddDynamic(this, &ATeleport::EnterTeleporter);
	OnActorEndOverlap.AddDynamic(this, &ATeleport::ExitTeleporter);

	elements.Add("Forest");
	elements.Add("Stone");
	elements.Add("Water");

	teleporting = false;
}

void ATeleport::BeginPlay() {
	Super::BeginPlay();
}

void ATeleport::EnterTeleporter(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		if (otherTele)
		{
			APlayerCharacter* character = Cast<APlayerCharacter>(otherActor);
			if (character && !otherTele->teleporting && elementNum != 3)
			{
				if (elementNum == 0) {
					character->isForest = true;
				}
				else if (elementNum == 1) {
					character->isStone = true;
				}
				else if (elementNum == 2) {
					character->isWater = true;
				}
				teleporting = true;
				character->SetActorRotation(otherTele->GetActorRotation());
				character->GetController()->SetControlRotation(character->GetActorRotation());
				character->SetActorLocation(otherTele->GetActorLocation());
			}
			else if (character && !otherTele->teleporting && elementNum == 3) {
				if (character->isForest == true && character->isStone == true && character->isWater == true) {
					teleporting = true;
					character->SetActorRotation(otherTele->GetActorRotation());
					character->GetController()->SetControlRotation(character->GetActorRotation());
					character->SetActorLocation(otherTele->GetActorLocation());
				}
			}
		}
	}
}

void ATeleport::ExitTeleporter(AActor* overlappedActor, AActor* otherActor)
{
	if (otherActor && otherActor != this)
	{
		if (otherTele && !teleporting)
		{
			otherTele->teleporting = false;
		}
	}
}
