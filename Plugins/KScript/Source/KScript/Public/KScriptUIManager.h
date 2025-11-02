// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "KScriptUIManager.generated.h"

class UKScriptImageManager;
class UKScriptAudioManager;
class UImage;
class UCanvasPanel;
class UTextBlock;

/**
 * UI統合管理システム - テキスト、画像、音声を統合管理
 */
UCLASS(BlueprintType, Blueprintable)
class KSCRIPT_API UKScriptUIManager : public UObject
{
	GENERATED_BODY()

public:
	UKScriptUIManager();

	/**
	 * 初期化
	 * @param InWorld ワールドコンテキスト
	 * @param InMessageText メッセージ表示用のTextBlock
	 * @param InBackgroundImage 背景用のImageウィジェット
	 * @param InCharacterContainer キャラクター用のコンテナウィジェット
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|UI")
	void Initialize(UWorld* InWorld, UTextBlock* InMessageText, UImage* InBackgroundImage, UCanvasPanel* InCharacterContainer);

	/**
	 * テキストを表示
	 * @param Text 表示するテキスト
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|UI")
	void DisplayText(const FString& Text);

	/**
	 * テキストを追加
	 * @param Text 追加するテキスト
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|UI")
	void AppendText(const FString& Text);

	/**
	 * テキストをクリア
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|UI")
	void ClearText();

	/**
	 * 改行を追加
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|UI")
	void AddLineBreak();

	/**
	 * 画像マネージャーを取得
	 */
	UFUNCTION(BlueprintPure, Category = "KScript|UI")
	UKScriptImageManager* GetImageManager() const { return ImageManager; }

	/**
	 * 音声マネージャーを取得
	 */
	UFUNCTION(BlueprintPure, Category = "KScript|UI")
	UKScriptAudioManager* GetAudioManager() const { return AudioManager; }

	/**
	 * 現在のメッセージテキストを取得
	 */
	UFUNCTION(BlueprintPure, Category = "KScript|UI")
	FString GetCurrentMessage() const { return CurrentMessage; }

protected:
	// 画像管理システム
	UPROPERTY()
	TObjectPtr<UKScriptImageManager> ImageManager;

	// 音声管理システム
	UPROPERTY()
	TObjectPtr<UKScriptAudioManager> AudioManager;

	// メッセージ表示用のTextBlock
	UPROPERTY()
	TObjectPtr<UTextBlock> MessageText;

	// 現在のメッセージ
	UPROPERTY()
	FString CurrentMessage;
};
