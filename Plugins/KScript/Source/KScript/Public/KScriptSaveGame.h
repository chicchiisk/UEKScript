// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "KScriptVariable.h"
#include "KScriptSaveGame.generated.h"

/**
 * 画像レイヤーの保存情報
 */
USTRUCT(BlueprintType)
struct KSCRIPT_API FKScriptSavedImageLayer
{
	GENERATED_BODY()

	UPROPERTY()
	FString TexturePath;

	UPROPERTY()
	FVector2D Position = FVector2D::ZeroVector;

	UPROPERTY()
	float Opacity = 1.0f;

	UPROPERTY()
	bool bIsVisible = false;
};

/**
 * KScriptのセーブデータ
 * ゲームの状態（変数、実行位置、画像・音声状態など）を保存
 */
UCLASS()
class KSCRIPT_API UKScriptSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UKScriptSaveGame();

	// ========================================
	// セーブデータの基本情報
	// ========================================

	/** セーブスロット名 */
	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	FString SlotName;

	/** セーブ日時 */
	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	FDateTime SaveDateTime;

	/** セーブ時のスクリーンショット（オプション） */
	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	UTexture2D* ScreenshotTexture = nullptr;

	/** プレイ時間（秒） */
	UPROPERTY(BlueprintReadWrite, Category = "SaveGame")
	float PlayTimeSeconds = 0.0f;

	// ========================================
	// スクリプト実行状態
	// ========================================

	/** 現在実行中のスクリプトアセットのパス */
	UPROPERTY(BlueprintReadWrite, Category = "Script")
	FString CurrentScriptAssetPath;

	/** 現在の実行コマンドインデックス */
	UPROPERTY(BlueprintReadWrite, Category = "Script")
	int32 CurrentCommandIndex = 0;

	/** コールスタック（サブルーチン用） */
	UPROPERTY(BlueprintReadWrite, Category = "Script")
	TArray<int32> CallStack;

	// ========================================
	// 変数データ
	// ========================================

	/** 保存された変数データ */
	UPROPERTY(BlueprintReadWrite, Category = "Variables")
	TMap<FString, FKScriptVariableValue> Variables;

	// ========================================
	// 画像・表示状態
	// ========================================

	/** 背景画像レイヤー */
	UPROPERTY(BlueprintReadWrite, Category = "Display")
	FKScriptSavedImageLayer BackgroundLayer;

	/** キャラクター画像レイヤー（キャラクター名 -> レイヤー情報） */
	UPROPERTY(BlueprintReadWrite, Category = "Display")
	TMap<FString, FKScriptSavedImageLayer> CharacterLayers;

	/** 現在表示中のメッセージテキスト */
	UPROPERTY(BlueprintReadWrite, Category = "Display")
	FString CurrentMessageText;

	// ========================================
	// 音声状態
	// ========================================

	/** 現在再生中のBGMのアセットパス */
	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	FString CurrentBGMPath;

	/** BGMの音量 */
	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	float BGMVolume = 1.0f;

	// ========================================
	// ユーティリティメソッド
	// ========================================

	/** セーブデータのダンプ情報を取得（デバッグ用） */
	UFUNCTION(BlueprintPure, Category = "SaveGame")
	FString GetSaveDataSummary() const;
};
