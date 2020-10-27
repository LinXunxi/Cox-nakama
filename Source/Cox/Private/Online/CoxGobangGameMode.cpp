// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/CoxGobangGameMode.h"

ACoxGobangGameMode::ACoxGobangGameMode()
{

}

void ACoxGobangGameMode::InitChessboard()
{
	ChessMap.Empty();
	for (int x = 0; x < 15; x++)
	{
		for (int y = 0; y < 15; y++)
		{
			ChessMap.Add(FVector(x,y,0), EChessType::Empty);
		}
	}
}

bool ACoxGobangGameMode::CheckWin(EChessType ChessType)
{
	int32 Count = 0;
	for (int x = 0; x < 15; x++)
	{
		for (int y = 0; y < 15; y++)
		{
			if (ChessMap[FVector(x, y, 0)] == ChessType)
			{
				for (Count = 1; Count < 5; Count++)
				{
					if (y + Count < 15)
					{
						if (ChessMap[FVector(x, y + Count, 0)] != ChessType)
						{
							break;
						}
					}
				}
				if (Count == 5)
				{
					return true;
				}

				for (Count = 1; Count < 5; Count++)
				{
					if (x + Count < 15)
					{
						if (ChessMap[FVector(x + Count, y, 0)] != ChessType)
						{
							break;
						}
					}
				}
				if (Count == 5)
				{
					return true;
				}

				for (Count = 1; Count < 5; Count++)
				{
					if (x + Count < 15 && y + Count < 15)
					{
						if (ChessMap[FVector(x + Count, y + Count, 0)] != ChessType)
						{
							break;
						}
					}
				}
				if (Count == 5)
				{
					return true;
				}
			}

		}
	}
	return false;
}

bool ACoxGobangGameMode::PressChess(float x, float y, EChessType ChessType)
{
	if (x < 15 && x >= 0 && y < 15 && y >= 0 && ChessMap.Contains(FVector(x, y, 0)))
	{
		if (ChessMap[FVector(x, y, 0)] == EChessType::Empty)
		{
			ChessMap[FVector(x, y, 0)] = ChessType;
			return true;
		}
	}
	return false;
}
