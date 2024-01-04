// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_InGame.h"

void UUI_InGame::set_HUDbar_value_percent(float value) {
	HUD_HPbar_value->SetPercent(value);
}