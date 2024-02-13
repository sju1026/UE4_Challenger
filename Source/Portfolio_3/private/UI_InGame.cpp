/*
* Writer : seo jae ung
*
* Function
* 
*
* UpdateRate : 2024 - 01 - 05
*/

#include "UI_InGame.h"

void UUI_InGame::set_HUDbar_value_percent(float value) {
	HUD_HPbar_value->SetPercent(value);
}

void UUI_InGame::set_HUDbar_Boss_value_percent(float value) {
	HUD_BossHPbar_value->SetPercent(value);
}