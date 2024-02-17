// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ABGameSingleton.generated.h"

/**
 * 
 */
UCLASS()
class PORTFOLIO_3_API UABGameSingleton : public UObject
{
	GENERATED_BODY()

public:
	UABGameSingleton();
	static UABGameSingleton& Get();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FName gameSingletonClassName;
};
