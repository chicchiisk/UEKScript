// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "KScriptUIManager.generated.h"

class UKScriptImageManager;
class UKScriptAudioManager;
class UImage;
class UCanvasPanel;
class UTextBlock;

/**
 * UI統合管理システム - テキスト、画像、音声を統合管理
 * GameInstanceSubsystemとして実装
 */
UCLASS()
class KSCRIPT_API UKScriptUIManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// USubsystem interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// End of USubsystem interface

	/**
	 * UIウィジェットを設定
	 * @param InMessageText メッセージ表示用のTextBlock
	 * @param InBackgroundImage 背景用のImageウィジェット
	 * @param InCharacterContainer キャラクター用のコンテナウィジェット
	 */
	UFUNCTION(BlueprintCallable, Category = "KScript|UI")
	void SetupWidgets(UTextBlock* InMessageText, UImage* InBackgroundImage, UCanvasPanel* InCharacterContainer);

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
	UKScriptImageManager* GetImageManager() const;

	/**
	 * 音声マネージャーを取得
	 */
	UFUNCTION(BlueprintPure, Category = "KScript|UI")
	UKScriptAudioManager* GetAudioManager() const;

	/**
	 * 現在のメッセージテキストを取得
	 */
	UFUNCTION(BlueprintPure, Category = "KScript|UI")
	FString GetCurrentMessage() const { return CurrentMessage; }

protected:
	// メッセージ表示用のTextBlock
	UPROPERTY()
	TObjectPtr<UTextBlock> MessageText;

	// 現在のメッセージ
	UPROPERTY()
	FString CurrentMessage;
};
