// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Portfolio_3GameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_3_API APortfolio_3GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	APortfolio_3GameModeBase();

	virtual void BeginPlay() override;
};
