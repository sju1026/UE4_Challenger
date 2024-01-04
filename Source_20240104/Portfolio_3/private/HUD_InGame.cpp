/*
* Writer : seo jae ung
*
* Function
*  - 인게임 HUD를 설정하는 클래스입니다. HUD는 게임 플레이어의 UI를 관리하며, 이 코드는 HUD에 사용할 UI 위젯을 설정합니다.
*  - 초기화 시, UI 위젯의 클래스를 찾아 설정하고, 게임이 시작되면 해당 UI 위젯을 생성하여 화면에 추가합니다.
*  
* UpdateRate : 2023 - 12 - 10
*/

#include "HUD_InGame.h"
#include <UObject/ConstructorHelpers.h>
#include <Blueprint/UserWidget.h>

AHUD_InGame::AHUD_InGame() {
	static ConstructorHelpers::FClassFinder<UUserWidget> ui(TEXT("WidgetBlueprint'/Game/_My/UI/WBP_GamePlayerUI.WBP_GamePlayerUI_C'"));
	if (ui.Succeeded()) {
		uiBPClass = ui.Class;
	}
}

void AHUD_InGame::BeginPlay()
{
	Super::BeginPlay();

	if (uiBPClass) {
		uiWidget = CreateWidget<UUserWidget>(GetWorld(), uiBPClass);
		if (uiWidget) {
			uiWidget->AddToViewport();
		}
	}
}
