// Fill out your copyright notice in the Description page of Project Settings.


#include "ABGameSingleton.h"

UABGameSingleton::UABGameSingleton() {
	UABGameSingleton* gameSingleton = this;

	if (gameSingleton == nullptr && gameSingletonClassName.ToString().Len() > 0) {
		UClass* singletonClass = LoadClass<UABGameSingleton>(nullptr, *gameSingletonClassName.ToString());

		if (singletonClass) {
			gameSingleton = NewObject<UABGameSingleton>(this, singletonClass);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Engine config value GameSingletoneClassName %s is not a valid class name."), *gameSingletonClassName.ToString());
		}
	}
}

UABGameSingleton& UABGameSingleton::Get()
{
	UABGameSingleton* singletone = CastChecked<UABGameSingleton>(GEngine->GameSingleton);
	if (singletone)
		return *singletone;

#if WITH_EDITOR
	UE_LOG(LogTemp, Error, TEXT("Invalid Game Singleton"));
#endif // WITH_EDITOR

	return *NewObject<UABGameSingleton>();

}
