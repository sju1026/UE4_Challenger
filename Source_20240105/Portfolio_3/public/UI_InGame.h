// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/ProgressBar.h>
#include "UI_InGame.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_3_API UUI_InGame : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void set_HUDbar_value_percent(float value);

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar* HUD_HPbar_value = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar* HUD_BossHPbar_value = nullptr;

	void set_HUDbar_Boss_value_percent(float value);

};
