// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UKScriptAsset;

/**
 * KScriptアセットのビューアーウィンドウ
 */
class SKScriptViewer : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SKScriptViewer) {}
		SLATE_ARGUMENT(UKScriptAsset*, KScriptAsset)
	SLATE_END_ARGS()

	/** ウィジェットを構築 */
	void Construct(const FArguments& InArgs);

	/** ウィンドウを作成して表示 */
	static void OpenViewer(UKScriptAsset* InAsset);

	/** Tick - ファイル変更を監視 */
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	/** 編集対象のKScriptアセット */
	TWeakObjectPtr<UKScriptAsset> KScriptAsset;

	/** テキストボックスに表示するテキスト */
	TSharedPtr<FString> ScriptTextPtr;

	/** VSCodeで開いている一時ファイルのパス */
	FString TempFilePath;

	/** 最後にチェックした一時ファイルのタイムスタンプ */
	FDateTime LastFileTimestamp;

	/** 一時ファイルが現在開かれているか */
	bool bTempFileOpen;

	/** VSCodeで開くボタンがクリックされた時の処理 */
	FReply OnOpenInVSCodeClicked();

	/** 保存ボタンがクリックされた時の処理 */
	FReply OnSaveClicked();

	/** 更新ボタンがクリックされた時の処理 */
	FReply OnRefreshClicked();

	/** テキストが変更された時の処理 */
	void OnTextChanged(const FText& NewText);

	/** テキストボックスのテキストを取得 */
	FText GetScriptText() const;

	/** 一時ファイルから変更を読み込む */
	void LoadFromTempFile();
};
