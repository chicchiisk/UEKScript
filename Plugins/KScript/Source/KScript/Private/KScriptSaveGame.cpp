// Copyright Epic Games, Inc. All Rights Reserved.

#include "KScriptSaveGame.h"
#include "KScript.h"

UKScriptSaveGame::UKScriptSaveGame()
{
	SlotName = TEXT("QuickSave");
	SaveDateTime = FDateTime::Now();
}

FString UKScriptSaveGame::GetSaveDataSummary() const
{
	FString Summary;
	Summary += FString::Printf(TEXT("=== KScript セーブデータ ===\n"));
	Summary += FString::Printf(TEXT("スロット名: %s\n"), *SlotName);
	Summary += FString::Printf(TEXT("セーブ日時: %s\n"), *SaveDateTime.ToString());
	Summary += FString::Printf(TEXT("プレイ時間: %.1f秒\n"), PlayTimeSeconds);
	Summary += FString::Printf(TEXT("スクリプト: %s\n"), *CurrentScriptAssetPath);
	Summary += FString::Printf(TEXT("実行位置: コマンド#%d\n"), CurrentCommandIndex);
	Summary += FString::Printf(TEXT("変数数: %d\n"), Variables.Num());
	Summary += FString::Printf(TEXT("キャラクター数: %d\n"), CharacterLayers.Num());
	Summary += FString::Printf(TEXT("BGM: %s\n"), CurrentBGMPath.IsEmpty() ? TEXT("なし") : *CurrentBGMPath);

	return Summary;
}
