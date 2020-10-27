// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CoxGobangGameMode.generated.h"


UENUM(BlueprintType)
enum class EChessType : uint8
{
	Empty = 0,
	White = 1,   
	Black = 2			  
};


UCLASS()
class COX_API ACoxGobangGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ACoxGobangGameMode();
	UFUNCTION(BlueprintCallable, Category = NAKAMA)
		void InitChessboard();
	UFUNCTION(BlueprintCallable, Category = NAKAMA)
		bool CheckWin(EChessType ChessType);
	UFUNCTION(BlueprintCallable, Category = NAKAMA)
		bool PressChess(float x, float y, EChessType ChessType);
public:
	UPROPERTY(BlueprintReadOnly, Category = NAKAMA)
		TMap<FVector, EChessType> ChessMap;
};
